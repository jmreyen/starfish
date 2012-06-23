#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "storyreport.h"
#include "setupdialog.h"
#include "storyreportdialog.h"

#include <QPrinter>
#include <QPrintDialog>
#include <QNetworkReply>
#include <QCheckBox>
#include <QItemDelegate>
#include <QTableWidget>


class MyDelegate : public QItemDelegate
{
public:
  void setEditorData ( QWidget * editor, const QModelIndex & index ) const;
  void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
};

void MyDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const
{
    if (index.column() == SP_BURN) {
        QListWidget *w = static_cast<QListWidget *>(editor);
        QVariantList list = index.data().toList();
        int row = 0;
        foreach (QVariant v, list) {
            w->item(row++)->setText(v.toString());
        }
    }
    else
        QItemDelegate::setEditorData(editor, index);
}
void MyDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
    if (index.column() == SP_BURN) {
        QListWidget *w = static_cast<QListWidget *>(editor);
        QVariantList list;
        for (int i=0; i<w->count(); ++i)
            list.append(w->item(i)->text().toInt());
        model->setData(index, list, Qt::EditRole);

    }
    else
        QItemDelegate::setModelData(editor, model, index);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    thePrinter(QPrinter::HighResolution),
    rpc(this),
    theUrl(),
    theSettings("pic.ini", QSettings::IniFormat)
{

    ui->setupUi(this);


    //setup Ticket Table
    ui->storyTable->setModel(&theStories);
    ui->storyTable->horizontalHeader()->show();
    theStoryDataMapper.setModel(&theStories);
    theStoryDataMapper.addMapping(ui->summaryEdit, ST_DESC);
    theStoryDataMapper.addMapping(ui->descriptionEdit, ST_NOTES);
    theStoryDataMapper.toFirst();
    connect(ui->storyTable->selectionModel(), SIGNAL(currentChanged (const QModelIndex & , const QModelIndex & )),
           SLOT(onStoryTableCurrentCellChanged(const QModelIndex & , const QModelIndex & )));
    connect(ui->storyTable->selectionModel(), SIGNAL(currentChanged ( const QModelIndex & , const QModelIndex &)),
            &theStoryDataMapper, SLOT(setCurrentModelIndex ( const QModelIndex & )));
    //setup burndown view
    ui->burnDownView->setScene(&theBurnDownScene);
    theBurnDownScene.setSceneRect(ui->burnDownView->rect());

    // setup card view
    ui->cardView->setScene(&theCardScene);
    theCardScene.setSceneRect(ui->cardView->rect());

    //Setup SprintTable
    ui->sprintTable->setModel(&theSprints);
    theSprintDataMapper.setItemDelegate(new MyDelegate());
    theSprintDataMapper.setModel(&theSprints);
    theSprintDataMapper.addMapping(ui->dueDateEdit, SP_DUE);
    theSprintDataMapper.addMapping(ui->capacityEdit, SP_CAP);
    theSprintDataMapper.addMapping(ui->velocityEdit, SP_VEL);
    theSprintDataMapper.addMapping(ui->workDayEdit, SP_DAYS);
    theSprintDataMapper.addMapping(ui->burnDownTable, SP_BURN);
    theSprintDataMapper.toFirst();

    //A different row in the sprint table is selected
    connect(ui->sprintTable->selectionModel(), SIGNAL(currentChanged ( const QModelIndex & , const QModelIndex &)),
            SLOT(onSprintTableCurrentCellChanged ( const QModelIndex & , const QModelIndex & )));
    connect(ui->sprintTable->selectionModel(), SIGNAL(currentChanged ( const QModelIndex & , const QModelIndex &)),
            &theSprintDataMapper, SLOT(setCurrentModelIndex ( const QModelIndex & )));
    connect(&theSprints, SIGNAL(dataChanged( const QModelIndex &, const QModelIndex &)),
            SLOT(onSprintModelDataChanged ( const QModelIndex & )));


    // Load Settings
    //TRAC
    theUrl.setUrl(theSettings.value("TRAC/Server").toString());
    theUrl.setPort(theSettings.value("TRAC/Port").toInt());
    theUrl.setUserName(theSettings.value("TRAC/UserName").toString());
    theUrl.setPassword(theSettings.value("TRAC/Password").toString());
    rpc.setUrl(theUrl);
    theQueryString = theSettings.value("TRAC/QueryString").toString();
    loadOnStart = theSettings.value("TRAC/LoadOnStart").toBool();

    //Columns of StoryTable
    int size = theSettings.beginReadArray("columns");
    for (int i = 0; i < size; ++i) {
        theSettings.setArrayIndex(i);
        bool show = theSettings.value(QString("show"), false).toBool();
        int width = theSettings.value(QString("width"), 100).toInt();
        ui->storyTable->setColumnHidden(i, !show);
        ui->storyTable->setColumnWidth(i, width==0?100:width);
    }


    theSettings.endArray();



    // setup Printer
    thePrinter.setOrientation(QPrinter::Landscape);
    thePrinter.setPageSize(QPrinter::A5);

    //if LoadOnStart flag ist set, load data
    if (loadOnStart)
        on_importButton_clicked();
}

MainWindow::~MainWindow()
{
   // Save Settings
   //TRAC
   theSettings.setValue("TRAC/Server", theUrl.toString(QUrl::RemoveUserInfo|QUrl::RemovePort));
   theSettings.setValue("TRAC/Port", theUrl.port());
   theSettings.setValue("TRAC/UserName", theUrl.userName());
   theSettings.setValue("TRAC/Password", theUrl.password());
   theSettings.setValue("TRAC/QueryString", theQueryString);
   theSettings.setValue("TRAC/LoadOnStart", loadOnStart);

   // Columns
   theSettings.beginWriteArray("columns");
   for (int i=0; i<theStories.columnCount(); ++i){
       theSettings.setArrayIndex(i);
       bool show = !ui->storyTable->isColumnHidden(i);
       ui->storyTable->setColumnHidden(i, false); // show column, otherwise width=0
       int width = ui->storyTable->columnWidth(i);
       theSettings.setValue(QString("show"), show);
       theSettings.setValue(QString("width"), width);
   }
   theSettings.endArray();
    delete ui;
}


void MainWindow::fillCard(int row, int col, StoryCardScene *scene)
{
    QString txt = theStories.data(row, col).toString();

    if (scene==0x0)
        scene = &theCardScene;

    if (theStories.rowCount()==0 || row == -1)
        return;

    switch (col) {
    case 0:
        txt = "Backlog Item #"  + txt;
        scene->setID(txt);
        break;
    case 1:
        scene->setDesc(txt);
        break;
    case 2:
        scene->setNotes(txt);
        break;
    case 3:
        scene->setHTD(txt);
        break;
    case 4:
        scene->setImp(txt);
        break;
    case 5:
        scene->setEst(txt);
        break;
    case 6:
        scene->setUser(txt);
        break;
    }
}

void MainWindow::fillCard(int row, StoryCardScene *scene)
{
    if (scene==0x0)
        scene = &theCardScene;
    fillCard(row, 0, scene);
    fillCard(row, 1, scene);
    fillCard(row, 2, scene);
    fillCard(row, 3, scene);
    fillCard(row, 4, scene);
    fillCard(row, 5, scene);
    fillCard(row, 6, scene);
}


void MainWindow::insertStoryRow(int id, const QString &sum, const QString &desc, const QString &htd, const QString &prio, const QString &est, const QString &usr, const QString &ms, const QString &stat)
{

    StoryData t(QString::number(id), sum, desc, htd, prio, est, usr, ms, stat);
    theStories.addTicket(t);

}


void MainWindow::onStoryTableCurrentCellChanged(const QModelIndex &current , const QModelIndex &previous )
{
    if (current.row()!= previous.row()) {
        fillCard(current.row());
//        ui->cardView->show();
    }
}

void MainWindow::onSprintTableCurrentCellChanged(const QModelIndex &current , const QModelIndex &previous )
{
    // This function only handles display operations, data input and sync ist done via "theSprintDataMapper"
    if (current.row()!= previous.row()) {
        const SprintData &d = theSprints.sprint(current.row());

        //set title of Sprint Groupbox
        ui->sprintDataGroupBox->setTitle(d.name());

        // enable/disable input when sprint is open/completed
        bool b = d.completed();
        ui->capacityEdit->setReadOnly(b);
        ui->dueDateEdit->setReadOnly(b);
        ui->velocityEdit->setReadOnly(b);
        ui->workDayEdit->setReadOnly(b);

        // setup size of BurndownTable
        QSize s(ui->burnDownTable->width()/(d.workDays()?d.workDays():1)-1, 20);
        ui->burnDownTable->clear();
        for (int i=0; i<d.workDays(); ++i){
            QListWidgetItem *item = new QListWidgetItem("0", ui->burnDownTable);
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            item->setSizeHint(s);
            ui->burnDownTable->addItem(item);
        }

        //set Data to BurnDownView
        theBurnDownScene.show(d.capacity(), d.workDays(), d.burnDown());

        //show only ticekts that belong to the selected sprint

    }
}

void MainWindow::onSprintModelDataChanged(const QModelIndex &index)
{
    //set Data to BurnDownView
    const SprintData &d = theSprints.sprint(index.row());
    theBurnDownScene.show(d.capacity(), d.workDays(), d.burnDown());
}

void MainWindow::on_addRowButton_clicked()
{
    insertStoryRow();
}

void MainWindow::on_removeRowButton_clicked()
{
//    int row = ui->storyTable->currentRow();
//    theScene.clearCard();
//    ui->storyTable->removeRow(row);
}

void MainWindow::on_printButton_clicked()
{

    QPrintDialog printDialog(&thePrinter);
    StoryCardScene scene;

    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&thePrinter);
        painter.setRenderHint(QPainter::Antialiasing);
        for (int i=0; i<theStories.rowCount(); ++i) {
//            if (((QCheckBox*)ui->storyTable->cellWidget(i,9))->isChecked()) {
                fillCard(i, &theCardScene);
                theCardScene.render(&painter);
                thePrinter.newPage();
            }
//        }
    }
}

void MainWindow::on_importButton_clicked()
{
    QVariantList ticketArgs;
    QVariantList sprintArgs;

    statusBar()->showMessage("Querying ...");

    //Query Tickets
    ticketArgs.append(theQueryString+"&max=0");
    rpc.call("ticket.query", ticketArgs,
                this, SLOT(ticketQueryResponseMethod(QVariant &)),
                this, SLOT(myFaultResponse(int, const QString &)));
    //Empty Ticket Table
    theStories.clear();
    //Set Ticket Filter to "All"
    ui->comboBox->setCurrentIndex(0);

    //Query Milestones/Sprints
    rpc.call("ticket.milestone.getAll", sprintArgs,
                this, SLOT(sprintQueryResponseMethod(QVariant &)),
                this, SLOT(myFaultResponse(int, const QString &)));

    theSprints.clear();
}


void MainWindow::ticketQueryResponseMethod(QVariant &arg) {
    QStringList list = arg.toStringList();
    QVariantList args, methodList;

    statusBar()->showMessage("Retrieving ...");
    for (int i=0; i<list.size(); ++i)    {
        QVariantMap newMethod;
        QVariantList newParams;
        newParams.append(list[i].toInt());
        newMethod.insert("methodName", "ticket.get");
        newMethod.insert("params", newParams);
        methodList.append(newMethod);
    }
    args.insert(0, methodList);;
    rpc.call("system.multicall", args, this, SLOT(getTicketResponseMethod(QVariant&)), this, SLOT(myFaultResponse(int, const QString &)));
}

void MainWindow::sprintQueryResponseMethod(QVariant &arg) {

    QStringList list = arg.toStringList();
    QVariantList args, methodList;

    for (int i=0; i<list.size(); ++i)    {
        QVariantMap newMethod;
        QVariantList newParams;
        newParams.append(list[i]);
        newMethod.insert("methodName", "ticket.milestone.get");
        newMethod.insert("params", newParams);
        methodList.append(newMethod);
    }
    args.insert(0, methodList);;
    rpc.call("system.multicall", args, this, SLOT(getSprintResponseMethod(QVariant&)), this, SLOT(myFaultResponse(int, const QString &)));
}

void MainWindow::getTicketResponseMethod(QVariant &arg)
{
    QVariantList ticketList = arg.toList();
    for (int i = 0; i < ticketList.size(); ++i) {
        QVariantList fieldList = ticketList[i].toList().at(0).toList();
        QMap<QString,QVariant> map = fieldList[3].toMap();
        insertStoryRow(
            fieldList[0].toInt(),
            map["summary"].toString(),
            map["description"].toString(),
            map["how_to_demo"].toString(),
            map["priority"].toString(),
            map["estimation"].toString(),
            map["reporter"].toString(),
            map["milestone"].toString(),
            getStatus(map));
    }
    statusBar()->showMessage("");
}

void MainWindow::getSprintResponseMethod(QVariant &arg)
{
    QVariantList sprintList = arg.toList();
    for (int i = 0; i < sprintList.size(); ++i) {
        QMap<QString, QVariant> map = sprintList[i].toList().at(0).toMap();
        QString nam = map["name"].toString();
        QDate dat = map["due"].toDateTime().date();
        bool cmp = map["completed"].toString()!= "0";
        QString dsc = map["description"].toString();
        SprintData s(nam, dat, cmp, dsc);
        theSprints.addSprint(s);
    }
    theSprints.sortByDate();
}



QString MainWindow::getStatus( QMap<QString,QVariant> &map) const {

    QString status = "unknown";
    if (map["status"].toString().left(8) == QString("new"))
        status = "new";
    else if (map["milestone"]==QString("waiting"))
        status = "waiting";
    else if (map["status"].toString().left(8) == QString("postponed"))
        status = "postponed";
    else if (map["status"].toString().left(8) == QString("closed"))
        status = "done";
    else if (map["milestone"]==QString("current"))
        status = "selected";
    else if ((map["status"]==QString("assigned") ||
              map["status"]==QString("accepted")) &&
             (map["milestone"]==QString("none") ||
              map["milestone"]==QString("")))
         status = "backlog";

    return status;
}


void MainWindow::myFaultResponse(int error, const QString &message) {
    QString msg = QString().sprintf("An Error occured: %i. Message: ", error) + message;
    qDebug() << msg;
    ui->statusBar->showMessage(msg, 5000);
}

void MainWindow::on_setupButton_clicked()
{
    SetupDialog dlg;
    dlg.setUrl(theUrl);
    dlg.setQueryString(theQueryString);
    dlg.setLoadOnStart(loadOnStart);
    for (int i=0; i<theStories.columnCount(); ++i)
        dlg.setShowColumn(i, !ui->storyTable->isColumnHidden(i));

    QObject::connect(&dlg, SIGNAL(accepted(QVariantMap)),
                     this, SLOT(onSetupAccepted(QVariantMap)));
    dlg.exec();
}

void MainWindow::onSetupAccepted(QVariantMap map)
{
    theUrl = map["Url"].toUrl();
    rpc.setUrl(theUrl);
    theQueryString = map["QueryString"].toString();
    QVariantList list = map["Columns"].toList();
    for (int i=0; i<list.size(); ++i)
        ui->storyTable->setColumnHidden(i, !list[i].toBool());
    loadOnStart = map["LoadOnStart"].toBool();
}


void MainWindow::onFilterRow(QString arg)
{
    for( int i = 0; i < theStories.rowCount(); ++i )
    {
        QString str = theStories.data(i,ST_STATUS).toString();
        if(arg == "all" || str.contains(arg) )
            ui->storyTable->setRowHidden( i, false);
        else
            ui->storyTable->setRowHidden( i, true);
    }
}


void MainWindow::on_reportButton_clicked()
{
   StoryReport d;
    d.beginInsertStory();
    for (int i=0; i<theStories.rowCount(); ++i) {
        d.insertStory(
            theStories.data(i,0).toInt(),
            theStories.data(i,1).toString(),
            theStories.data(i,2).toString(),
            theStories.data(i,3).toString(),
            theStories.data(i,4).toString(),
            theStories.data(i,5).toString(),
            theStories.data(i,6).toString(),
            theStories.data(i,8).toString());

    }
    d.endInsertStory();

    ReportDialog dlg;
    dlg.setTextDocument(&d);
    dlg.exec();

}

void MainWindow::on_filterBySprintCheckBox_clicked(bool checked)
{
    QModelIndexList indexList = ui->sprintTable->selectionModel()->selection().indexes();
    QStringList stringList;
    foreach (QModelIndex index, indexList)
        stringList.append(theSprints.sprint(index.row()).name());
    for( int i = 0; i < theStories.rowCount(); ++i )
    {
        if (checked) {
            QString str = theStories.data(i,ST_SPRINT).toString();
            ui->storyTable->setRowHidden( i, !stringList.contains(str));
        }
        else
            ui->storyTable->setRowHidden( i, false);
    }
}
