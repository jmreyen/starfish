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




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    thePrinter(QPrinter::HighResolution),
    theSettings("pic.ini", QSettings::IniFormat)
{
    ui->setupUi(this);
    theLoader = new TracDataLoader(theStories, theSprints,
              ui->impComboBox->model(),ui->estComboBox->model(),
              ui->comComboBox->model(),ui->verComboBox->model(),
              ui->typComboBox->model(), ui->filterByStatusComboBox->model(), this);

    //*** story view ***
    //setup story table
    ui->storyTable->setModel(&theStories);
    ui->storyTable->horizontalHeader()->show();
    //map story table entries to editor widgets
    theStoryDataMapper.setModel(&theStories);
    theStoryDataMapper.setItemDelegate(new StoryModelItemDelegate());
    theStoryDataMapper.addMapping(ui->summaryEdit, ST_DESC);
    theStoryDataMapper.addMapping(ui->reporterEdit, ST_USER);
    theStoryDataMapper.addMapping(ui->descriptionEdit, ST_NOTES);
    theStoryDataMapper.addMapping(ui->estComboBox, ST_EST);
    theStoryDataMapper.addMapping(ui->impComboBox, ST_IMP);
    theStoryDataMapper.addMapping(ui->typComboBox, ST_TYP);
    theStoryDataMapper.addMapping(ui->comComboBox, ST_COMP);
    theStoryDataMapper.addMapping(ui->verComboBox, ST_VERSION);
    theStoryDataMapper.toFirst();
    //signals and slots for the story table
    connect(ui->storyTable->selectionModel(), SIGNAL(currentChanged (const QModelIndex & , const QModelIndex & )),
           SLOT(onStoryTableCurrentCellChanged(const QModelIndex & , const QModelIndex & )));
    connect(ui->storyTable->selectionModel(), SIGNAL(currentChanged ( const QModelIndex & , const QModelIndex &)),
            &theStoryDataMapper, SLOT(setCurrentModelIndex ( const QModelIndex & )));
    connect(&theStories, SIGNAL(dataChanged( const QModelIndex &, const QModelIndex &)),
            SLOT(onStoryModelDataChanged ( const QModelIndex & )));
    connect(&theStories, SIGNAL(dataChanged( const QModelIndex &, const QModelIndex &)),
            &theStoryDataMapper, SLOT(setCurrentModelIndex ( const QModelIndex & )));
    connect(&theStories, SIGNAL(layoutChanged()),
            SLOT(onStoryTableLayoutChanged()));
    connect(&theStories, SIGNAL(layoutAboutToBeChanged()),
            SLOT(onStoryTableLayoutAboutToBeChanged()));
    // *** sprint view ***
    //setup SprintTable
    ui->sprintTable->setModel(&theSprints);
    //setup burndown view
    ui->burnDownView->setScene(&theBurnDownScene);
    theBurnDownScene.setSceneRect(ui->burnDownView->rect());
    //map sprint table entries to edit widgets
    theSprintDataMapper.setItemDelegate(new SprintModelItemDelegate());
    theSprintDataMapper.setModel(&theSprints);
    theSprintDataMapper.addMapping(ui->dueDateEdit, SP_DUE);
    theSprintDataMapper.addMapping(ui->capacityEdit, SP_CAP);
    theSprintDataMapper.addMapping(ui->velocityEdit, SP_VEL);
    theSprintDataMapper.addMapping(ui->workDayEdit, SP_DAYS);
    theSprintDataMapper.addMapping(ui->burnDownTable, SP_BURN);
    theSprintDataMapper.toFirst();
    //Signals & Slots for the sprint table and the sprint model
    connect(ui->sprintTable->selectionModel(), SIGNAL(currentChanged ( const QModelIndex & , const QModelIndex &)),
            SLOT(onSprintTableCurrentCellChanged ( const QModelIndex & , const QModelIndex & )));
    connect(ui->sprintTable->selectionModel(), SIGNAL(currentChanged ( const QModelIndex & , const QModelIndex &)),
            &theSprintDataMapper, SLOT(setCurrentModelIndex ( const QModelIndex & )));
    connect(&theSprints, SIGNAL(dataChanged( const QModelIndex &, const QModelIndex &)),
            SLOT(onSprintModelDataChanged ( const QModelIndex & )));
    // *** Index Card View ***
    // setup card view
    ui->cardView->setScene(&theCardScene);
    theCardScene.setSceneRect(ui->cardView->rect());
    // *** Load Settings ***
    //TRAC settings
    QUrl url;
    url.setUrl(theSettings.value("TRAC/Server").toString());
    url.setPort(theSettings.value("TRAC/Port").toInt());
    url.setUserName(theSettings.value("TRAC/UserName").toString());
    url.setPassword(theSettings.value("TRAC/Password").toString());
    theLoader->setUrl(url);
    theLoader->setQueryString(theSettings.value("TRAC/QueryString").toString());
    loadOnStart = theSettings.value("TRAC/LoadOnStart").toBool();
    //Layout settings
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
    // if LoadOnStart flag ist set, load data
    if (loadOnStart)
        on_importButton_clicked();
}

MainWindow::~MainWindow()
{
   // Save Settings
   //TRAC
   theSettings.setValue("TRAC/Server", theLoader->url().toString(QUrl::RemoveUserInfo|QUrl::RemovePort));
   theSettings.setValue("TRAC/Port", theLoader->url().port());
   theSettings.setValue("TRAC/UserName", theLoader->url().userName());
   theSettings.setValue("TRAC/Password", theLoader->url().password());
   theSettings.setValue("TRAC/QueryString", theLoader->queryString());
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




void MainWindow::onStoryTableCurrentCellChanged(const QModelIndex &current , const QModelIndex &previous )
{
    if (current.row()!= previous.row()) {
        fillCard(current.row());
    }
}

void MainWindow::onStoryModelDataChanged(const QModelIndex &index)
{
    fillCard(index.row());
    theStoryChanges.append(index);
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

QVariantList tmpList;
void MainWindow::onStoryTableLayoutAboutToBeChanged()
{
    // Store hidden rows
    for( int i = 0; i < theStories.rowCount(); ++i ) {
        if (ui->storyTable->isRowHidden(i))
            tmpList.append(theStories.data(i, ST_ID, Qt::DisplayRole));
    }
}

void MainWindow::onStoryTableLayoutChanged()
{
    // Restore hidden rows
    for( int i = 0; i < theStories.rowCount(); ++i ) {
        if (tmpList.contains(theStories.data(i, ST_ID, Qt::DisplayRole)))
            ui->storyTable->setRowHidden(i, true);
        else
            ui->storyTable->setRowHidden(i, false);
    }
    tmpList.clear();
}


void MainWindow::on_addRowButton_clicked()
{
    theStories.addNewStory();
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
            if (theStories.printFlag(i)) {
                fillCard(i, &theCardScene);
                theCardScene.render(&painter);
                thePrinter.newPage();
            }
        }
    }
}

void MainWindow::on_importButton_clicked()
{
    theSprints.clear();
    theStories.clear();
    ui->filterByStatusComboBox->setCurrentIndex(0);
    statusBar()->showMessage("Querying ...");
    theLoader->loadMasterData();
    theLoader->loadStories();
    theLoader->loadSprints();
    theSprints.sortByDate();

}


void MainWindow::on_setupButton_clicked()
{
    SetupDialog dlg;
    dlg.setUrl(theLoader->url());
    dlg.setQueryString(theLoader->queryString());
    dlg.setLoadOnStart(loadOnStart);
    for (int i=0; i<theStories.columnCount(); ++i)
        dlg.setShowColumn(i, !ui->storyTable->isColumnHidden(i));

    QObject::connect(&dlg, SIGNAL(accepted(QVariantMap)),
                     this, SLOT(onSetupAccepted(QVariantMap)));
    dlg.exec();
}

void MainWindow::onSetupAccepted(QVariantMap map)
{
    theLoader->setUrl(map["Url"].toUrl());
    theLoader->setQueryString(map["QueryString"].toString());
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

void MainWindow::on_saveStoryButton_clicked()
{
    theLoader->saveNewStories();
//    qDebug() << "*** Changes ***\n";
//    foreach (QPersistentModelIndex i, theStoryChanges)
//        qDebug() << "row: " << i.row() << "col: " << i.column() << "data: " << theStories.data(i).toString() << "\n" ;
}
