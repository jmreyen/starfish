#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ticketreport.h"
#include "setupdialog.h"
#include "ticketreportdialog.h"

#include <QPrinter>
#include <QPrintDialog>
#include <QNetworkReply>
#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    theScene(0,0,298,210,this),
    thePrinter(QPrinter::HighResolution),
    rpc(this),
    theUrl(),
    theSettings("pic.ini", QSettings::IniFormat)
{

    ui->setupUi(this);

    //setup Ticket Table
    ui->storyTable->setModel(&theTickets);

    // setup card view
    ui->cardView->setScene(&theScene);

    //Get Updates from StoryTable
    connect(ui->storyTable->selectionModel(), SIGNAL(currentChanged (const QModelIndex & , const QModelIndex & )),
           SLOT(onStoryTableCurrentCellChanged(const QModelIndex & , const QModelIndex & )));

    // Load Settings
    //TRAC
    theUrl.setUrl(theSettings.value("TRAC/Server").toString());
    theUrl.setPort(theSettings.value("TRAC/Port").toInt());
    theUrl.setUserName(theSettings.value("TRAC/UserName").toString());
    theUrl.setPassword(theSettings.value("TRAC/Password").toString());
    rpc.setUrl(theUrl);
    theQueryString = theSettings.value("TRAC/QueryString").toString();

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

    ui->storyTable->horizontalHeader()->show();

    //Setup SprintTable
    ui->sprintTable->setColumnHidden(1, true);
    ui->sprintTable->setColumnHidden(2, true);
    ui->sprintTable->sortByColumn(1, Qt::DescendingOrder);

    // setup Printer
    thePrinter.setOrientation(QPrinter::Landscape);
    thePrinter.setPageSize(QPrinter::A5);
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

   // Columns
   theSettings.beginWriteArray("columns");
   for (int i=0; i<theTickets.columnCount(); ++i){
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
    QString txt = theTickets.data(row, col).toString();

    if (scene==0x0)
        scene = &theScene;

    if (theTickets.rowCount()==0 || row == -1)
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
        scene = &theScene;
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

    TicketData t(QString::number(id), sum, desc, htd, prio, est, usr, ms, stat);
    theTickets.addTicket(t);

}


void MainWindow::onStoryTableCurrentCellChanged(const QModelIndex &current , const QModelIndex &previous )
{
    if (current.row()!= previous.row()) {
        fillCard(current.row());
        ui->cardView->show();
    }
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
        for (int i=0; i<theTickets.rowCount(); ++i) {
//            if (((QCheckBox*)ui->storyTable->cellWidget(i,9))->isChecked()) {
                fillCard(i, &theScene);
                theScene.render(&painter);
                thePrinter.newPage();
            }
//        }
    }
}

QMutex mutex;

void MainWindow::on_importButton_clicked()
{
    QVariantList ticketArgs;
    QVariantList sprintArgs;

    statusBar()->showMessage("Querying ...");

    //Query Tickets
    ticketArgs.append(theQueryString);
    rpc.call("ticket.query", ticketArgs,
                this, SLOT(ticketQueryResponseMethod(QVariant &)),
                this, SLOT(myFaultResponse(int, const QString &)));
    //Empty Ticket Table
    theTickets.clear();
    //Set Ticket Filter to "All"
    ui->comboBox->setCurrentIndex(0);

    //Query Milestones/Sprints
    rpc.call("ticket.milestone.getAll", sprintArgs,
                this, SLOT(sprintQueryResponseMethod(QVariant &)),
                this, SLOT(myFaultResponse(int, const QString &)));

    ui->sprintTable->clear();
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
    rpc.call("system.multicall", args, this, SLOT(getSprintResponseMethod(QVariant&)), this, SLOT(myFaultResponse(int, const QString &)));}

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
    ui->storyTable->resizeRowsToContents ();
    statusBar()->showMessage("");
}

void MainWindow::getSprintResponseMethod(QVariant &arg)
{
    QVariantList sprintList = arg.toList();
    for (int i = 0; i < sprintList.size(); ++i) {
        QMap<QString, QVariant> map = sprintList[i].toList().at(0).toMap();
        int row = ui->sprintTable->rowCount();
        ui->sprintTable->insertRow(row);
        ui->sprintTable->setItem(row,0, new QTableWidgetItem(map["name"].toString()));
        ui->sprintTable->setItem(row,1, new QTableWidgetItem(map["due"].toString()));
        ui->sprintTable->setItem(row,2, new QTableWidgetItem(map["completed"].toString()));
    }
    ui->sprintTable->resizeRowsToContents ();
}



QString MainWindow::getStatus( QMap<QString,QVariant> &map) const {

    QString status = "unknown";
    if (map["status"].toString().left(8) == QString("new"))
        status = "new";
    else if (map["milestone"]==QString("waiting"))
        status = "waiting";
    if (map["status"].toString().left(8) == QString("postponed"))
        status = "postponed";
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
    for (int i=0; i<theTickets.columnCount(); ++i)
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
}


void MainWindow::onFilterRow(QString arg)
{
    for( int i = 0; i < theTickets.rowCount(); ++i )
    {
        QString str = theTickets.data(i,8).toString();
        if(arg == "all" || str.contains(arg) )
            ui->storyTable->setRowHidden( i, false);
        else
            ui->storyTable->setRowHidden( i, true);
    }
}


void MainWindow::on_reportButton_clicked()
{
    TicketReport d;
    d.beginInsertTicket();
    for (int i=0; i<theTickets.rowCount(); ++i) {
        d.insertTicket(
            theTickets.data(i,0).toInt(),
            theTickets.data(i,1).toString(),
            theTickets.data(i,2).toString(),
            theTickets.data(i,3).toString(),
            theTickets.data(i,4).toString(),
            theTickets.data(i,5).toString(),
            theTickets.data(i,6).toString(),
            theTickets.data(i,8).toString());

    }
    d.endInsertTicket();

    ReportDialog dlg;
    dlg.setTextDocument(&d);
    dlg.exec();

}
