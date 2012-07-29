#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "storyitemdelegate.h"
#include "storyreport.h"
#include "setupdialog.h"
#include "storyreportdialog.h"
#include "newstorydialog.h"

#include <QPrinter>
#include <QPrintDialog>
#include <QNetworkReply>
#include <QCheckBox>
#include <QItemDelegate>
#include <QTableWidget>
#include <QMessageBox>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    theStoryTree(this),
    theSprints(this),
    theStoryDataMapper(this),
    theSprintDataMapper(this),
    theBurnDownScene(this),
    theCardScene(this),
    thePrinter(QPrinter::HighResolution),
    theSettings("pic.ini", QSettings::IniFormat),
    loadOnStart(false)
{
    ui->setupUi(this);

    //*** Setup MainWindow Elements ***
    // conect slots for performing toolbar actions
    connect(ui->actionAdd_Story, SIGNAL(triggered()), SLOT(onActionAddStory()));
    connect(ui->actionLoad, SIGNAL(triggered()), SLOT(onActionLoad()));
    connect(ui->actionStore, SIGNAL(triggered()), SLOT(onActionStoreStories()));
    connect(ui->actionSettings, SIGNAL(triggered()), SLOT(onActionSettings()));
    connect(ui->actionShow_Report, SIGNAL(triggered()), SLOT(onActionReport()));
    connect(ui->actionPrint, SIGNAL(triggered()), SLOT(onActionPrint()));
    //setup story tree
    ui->storyTreeView->setModel(&theStoryTree);
    ui->storyTreeView->setDragEnabled (true );
    ui->storyTreeView->setAcceptDrops (true );
    ui->storyTreeView->setDropIndicatorShown (true );

    //map story table entries to editor widgets
    theStoryDataMapper.setModel(&theStoryTree);
    theStoryDataMapper.setItemDelegate(new StoryItemDelegate());
    theStoryDataMapper.addMapping(ui->summaryEdit, ST_DESC);
    theStoryDataMapper.addMapping(ui->reporterEdit, ST_USER);
    theStoryDataMapper.addMapping(ui->descriptionEdit, ST_NOTES);
    theStoryDataMapper.addMapping(ui->estComboBox, ST_EST);
    theStoryDataMapper.addMapping(ui->impComboBox, ST_IMP);
    theStoryDataMapper.addMapping(ui->sprComboBox, ST_SPRINT);
    theStoryDataMapper.addMapping(ui->typComboBox, ST_TYP);
    theStoryDataMapper.addMapping(ui->comComboBox, ST_COMP);
    theStoryDataMapper.addMapping(ui->verComboBox, ST_VERSION);
    theStoryDataMapper.toFirst();
    //connect slots for story table events
    connect(ui->storyTreeView->selectionModel(), SIGNAL(currentChanged (const QModelIndex & , const QModelIndex & )),
            SLOT(onStoryTableCurrentCellChanged(const QModelIndex & , const QModelIndex & )));
//    connect(ui->storyTreeView->selectionModel(), SIGNAL(currentChanged ( const QModelIndex & , const QModelIndex &)),
//            &theStoryDataMapper, SLOT(setCurrentModelIndex ( const QModelIndex & )));
    connect(&theStoryTree, SIGNAL(dataChanged( const QModelIndex &, const QModelIndex &)),
            SLOT(onStoryModelDataChanged ( const QModelIndex & )));
    connect(&theStoryTree, SIGNAL(dataChanged( const QModelIndex &, const QModelIndex &)),
            &theStoryDataMapper, SLOT(setCurrentModelIndex ( const QModelIndex & )));
    connect(&theStoryTree, SIGNAL(layoutChanged()),
            SLOT(onStoryTableLayoutChanged()));
    connect(&theStoryTree, SIGNAL(layoutAboutToBeChanged()),
            SLOT(onStoryTableLayoutAboutToBeChanged()));
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
    //Connect slots for the sprint table and the sprint model
    connect(ui->sprintTable->selectionModel(), SIGNAL(currentChanged ( const QModelIndex & , const QModelIndex &)),
            SLOT(onSprintTableCurrentCellChanged ( const QModelIndex & , const QModelIndex & )));
    connect(ui->sprintTable->selectionModel(), SIGNAL(currentChanged ( const QModelIndex & , const QModelIndex &)),
            &theSprintDataMapper, SLOT(setCurrentModelIndex ( const QModelIndex & )));
    connect(&theSprints, SIGNAL(dataChanged( const QModelIndex &, const QModelIndex &)),
            SLOT(onSprintModelDataChanged ( const QModelIndex & )));
    // setup card view
    ui->cardView->setScene(&theCardScene);
    theCardScene.setSceneRect(ui->cardView->rect());

    // *** Setup Loader ***
    theLoader = IOFactory::Instance()->getIO("TRAC");

    // *** Load Settings ***
    //I/O settings
    loadOnStart = theSettings.value("IO/LoadOnStart").toBool();
    theLoader->loadSettings(theSettings);
    //Layout settings
    int size = theSettings.beginReadArray("columns");
    for (int i = 0; i < size; ++i) {
        theSettings.setArrayIndex(i);
        bool show = theSettings.value(QString("show"), false).toBool();
        int width = theSettings.value(QString("width"), 100).toInt();
        ui->storyTreeView->setColumnHidden(i, !show);
        ui->storyTreeView->setColumnWidth(i, width==0?100:width);
    }
    theSettings.endArray();

    // connect slots for loading data
    connect(theLoader, SIGNAL(storiesLoaded(const QVariantList &)),
            SLOT(setStories(const QVariantList &)));
    connect(theLoader, SIGNAL(sprintsLoaded(const QVariantList &)),
            SLOT(setSprints(const QVariantList &)));
    connect(theLoader, SIGNAL(estimationsLoaded(const QStringList &)),
            SLOT(setEstimations(const QStringList &)));
    connect(theLoader, SIGNAL(prioritiesLoaded(const QStringList &)),
            SLOT(setPriorities(const QStringList &)));
    connect(theLoader, SIGNAL(versionsLoaded(const QStringList &)),
            SLOT(setVersions(const QStringList &)));
    connect(theLoader, SIGNAL(typesLoaded(const QStringList &)),
            SLOT(setTypes(const QStringList &)));
    connect(theLoader, SIGNAL(componentsLoaded(const QStringList &)),
            SLOT(setComponents(const QStringList &)));
    connect(theLoader, SIGNAL(newStoryLoaded(const QVariantMap &)),
            SLOT(addNewlySavedStory(const QVariantMap &)));

    // setup Printer
    thePrinter.setOrientation(QPrinter::Landscape);
    thePrinter.setPageSize(QPrinter::A5);
    // if LoadOnStart flag ist set, load data
    if (loadOnStart)
        loadAll();
}

MainWindow::~MainWindow()
{
    // Save Settings
    //I/O
    theSettings.setValue("IO/LoadOnStart", loadOnStart);
    theLoader->saveSettings(theSettings);
    // Columns
    theSettings.beginWriteArray("columns");
    for (int i=0; i<theStoryTree.columnCount(); ++i){
        theSettings.setArrayIndex(i);
        bool show = !ui->storyTreeView->isColumnHidden(i);
        ui->storyTreeView->setColumnHidden(i, false); // show column, otherwise width=0
        int width = ui->storyTreeView->columnWidth(i);
        theSettings.setValue(QString("show"), show);
        theSettings.setValue(QString("width"), width);
    }
    theSettings.endArray();
    delete ui;
}


void MainWindow::loadAll()
{
    theSprints.clear();
    theStoryTree.clear();
    ui->impComboBox->clear();
    ui->estComboBox->clear();
    ui->comComboBox->clear();
    ui->verComboBox->clear();
    ui->typComboBox->clear();
    ui->filterByStatusComboBox->clear();
    ui->filterByStatusComboBox->addItem("all");
    statusBar()->showMessage("Querying ...");
    theLoader->load();
    ui->filterByStatusComboBox->setCurrentIndex(0);
    theSprints.sortByDate();

}

void MainWindow::fillCard(const QModelIndex &index, StoryCardScene *scene)
{
    if (theStoryTree.rowCount()==0 || !index.isValid())
        return;

    if (scene==0x0)
        scene = &theCardScene;

    QString txt;
    QModelIndex currentIndex = theStoryTree.index(index.row(), 0, index.parent());
    txt = "Backlog Item #" + theStoryTree.data(currentIndex).toString();
    scene->setID(txt);

    currentIndex = theStoryTree.index(index.row(), 1, index.parent());
    txt = theStoryTree.data(currentIndex).toString();
    scene->setDesc(txt);

    currentIndex = theStoryTree.index(index.row(), 2, index.parent());
    txt = theStoryTree.data(currentIndex).toString();
    scene->setNotes(txt);

    currentIndex = theStoryTree.index(index.row(), 3, index.parent());
    txt = theStoryTree.data(currentIndex).toString();
    scene->setHTD(txt);

    currentIndex = theStoryTree.index(index.row(), 4, index.parent());
    txt = theStoryTree.data(currentIndex).toString();
    scene->setImp(txt);

    currentIndex = theStoryTree.index(index.row(), 5, index.parent());
    txt = theStoryTree.data(currentIndex).toString();
    scene->setEst(txt);

    currentIndex = theStoryTree.index(index.row(), 6, index.parent());
    txt = theStoryTree.data(currentIndex).toString();
    scene->setUser(txt);
}


void MainWindow::onActionAddStory()
{
    // determine parent ...
    QModelIndex currentIndex = ui->storyTreeView->currentIndex();
    QString parentId = theStoryTree.data(
        theStoryTree.index(
            currentIndex.row(),
            ST_ID,
            currentIndex.parent()
        )
    ).toString();

    NewStoryDialog dlg(ui->estComboBox->model(),
                       ui->impComboBox->model(),
                       ui->sprComboBox->model(),
                       ui->typComboBox->model(),
                       ui->comComboBox->model(),
                       ui->verComboBox->model(),
                       parentId);

    QObject::connect(&dlg, SIGNAL(accepted(const QVariantMap &)),
                     theLoader, SLOT(onSaveNewStory(const QVariantMap &)));

    dlg.exec();
}

void MainWindow::onActionStoreStories()
{
    if (theStoryChanges.isEmpty())
        return;

    QMap<QString, QVariantMap> updateMap;
    foreach (QPersistentModelIndex i, theStoryChanges) {
        QString id = theStoryTree.data(theStoryTree.index(i.row(), ST_ID, i.parent()), Qt::DisplayRole).toString();
        QString attribute = storyFieldNames[i.column()];
        updateMap[id][attribute] = theStoryTree.data(i);
    }

    QString changedStories;
    for (QMap<QString, QVariantMap>::iterator itr = updateMap.begin(); itr != updateMap.end(); ++itr) {
        changedStories += (changedStories.isEmpty()?"":", ") + itr.key();
//        changedStories += "(";
//        for (QMap<QString, QVariant>::iterator itr2 = itr.value().begin(); itr2 != itr.value().end(); ++itr2)
//            changedStories += itr2.key() + " = " + itr2.value().toString();
//        changedStories += ")";

    }

    QMessageBox msgBox;
    msgBox.setText(QString("The following stories have been modified: ")+changedStories);
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Save:
        theLoader->updateStories(updateMap);
        theStoryChanges.clear();
        break;
    case QMessageBox::Discard:
    case QMessageBox::Cancel:
    default:
        break;
    }
}

void MainWindow::onActionLoad()
{
    loadAll();
}

void MainWindow::onActionSettings()
{
    SetupDialog dlg(theLoader->getSettingsFrame());
    dlg.setLoadOnStart(loadOnStart);
    for (int i=0; i<theStoryTree.columnCount(); ++i)
        dlg.setShowColumn(i, !ui->storyTreeView->isColumnHidden(i));

    QObject::connect(&dlg, SIGNAL(accepted(const QVariantMap &)),
                     this, SLOT(onSetupAccepted(const QVariantMap &)));

    QObject::connect(&dlg, SIGNAL(accepted(const QVariantMap &)),
                     theLoader, SLOT(onSetupAccepted()));

    dlg.exec();
}

void MainWindow::onSetupAccepted(const QVariantMap &map)
{
    QVariantList list = map["Columns"].toList();
    for (int i=0; i<list.size(); ++i)
        ui->storyTreeView->setColumnHidden(i, !list[i].toBool());
    loadOnStart = map["LoadOnStart"].toBool();
}

void MainWindow::onActionReport()
{
//    StoryReport d;
//    d.beginInsertStory();
//    for (int i=0; i<theStoryTree.rowCount(); ++i) {
//        d.insertStory(
//                    theStoryTree.data(i,0).toInt(),
//                    theStoryTree.data(i,1).toString(),
//                    theStoryTree.data(i,2).toString(),
//                    theStoryTree.data(i,3).toString(),
//                    theStoryTree.data(i,4).toString(),
//                    theStoryTree.data(i,5).toString(),
//                    theStoryTree.data(i,6).toString(),
//                    theStoryTree.data(i,8).toString());

//    }
//    d.endInsertStory();

//    ReportDialog dlg;
//    dlg.setTextDocument(&d);
//    dlg.exec();

}

void MainWindow::onActionPrint()
{

    QPrintDialog printDialog(&thePrinter);
    StoryCardScene scene;

    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&thePrinter);
        painter.setRenderHint(QPainter::Antialiasing);

        StoryCardScene scene;
        for (StoryItemModel::iterator itr=theStoryTree.begin(); itr != theStoryTree.end(); ++itr) {
            if (itr->data(ST_PRINT).toBool()) {
                scene.setID(QString ("Backlog Item #" + itr->data(ST_ID).toString()));
                scene.setDesc(itr->data(ST_DESC).toString());
                scene.setNotes(itr->data(ST_NOTES).toString());
                scene.setHTD(itr->data(ST_HTD).toString());
                scene.setImp(itr->data(ST_IMP).toString());
                scene.setEst(itr->data(ST_EST).toString());
                scene.setUser(itr->data(ST_USER).toString());

                scene.render(&painter);
                thePrinter.newPage();
            }
        }
    }
}



void MainWindow::onStoryTableCurrentCellChanged(const QModelIndex &current , const QModelIndex &previous )
{
    if (current!= previous) {
        fillCard(current);
        // if the root index is not set, the mapper won't findy the sub items
        theStoryDataMapper.setRootIndex(current.parent());
        theStoryDataMapper.setCurrentModelIndex(current);
    }
}

void MainWindow::onStoryModelDataChanged(const QModelIndex &index)
{
    fillCard(index);
    theStoryChanges.append(index);
}

QVariantList tmpList;
void MainWindow::onStoryTableLayoutAboutToBeChanged() const
{
    // Store hidden rows
    for (StoryItemModel::iterator itr=theStoryTree.begin(); itr != theStoryTree.end(); ++itr) {
        QModelIndex index = itr.currentIndex();
        if (ui->storyTreeView->isRowHidden(index.row(), theStoryTree.parent(index))) {
            QModelIndex idIndex = theStoryTree.index(index.row(), ST_ID, index.parent());
            QString id = theStoryTree.data(idIndex, Qt::DisplayRole).toString();
            tmpList.append(id);
        }
    }
}

void MainWindow::onStoryTableLayoutChanged()
{
    // Restore hidden rows
    for (StoryItemModel::iterator itr=theStoryTree.begin(); itr != theStoryTree.end(); ++itr) {
        QModelIndex index = itr.currentIndex();
        QModelIndex idIndex = theStoryTree.index(index.row(), ST_ID, index.parent());
        if (tmpList.contains(theStoryTree.data(idIndex, Qt::DisplayRole)))
            ui->storyTreeView->setRowHidden(index.row(),theStoryTree.parent(index), true);
        else
            ui->storyTreeView->setRowHidden(index.row(),theStoryTree.parent(index), false);
    }
    tmpList.clear();
}

void MainWindow::onFilterRow(QString arg)
{
//    for( int i = 0; i < theStoryTree.rowCount(); ++i )
//    {
//        QString str = theStoryTree.data(i,ST_STATUS).toString();
//        if(arg == "all" || str.contains(arg) )
//            ui->storyTreeView->setRowHidden( i, false);
//        else
//            ui->storyTreeView->setRowHidden( i, true);
//    }
}

void MainWindow::on_filterBySprintCheckBox_clicked(bool checked)
{
//    QModelIndexList indexList = ui->sprintTable->selectionModel()->selection().indexes();
//    QStringList stringList;
//    foreach (QModelIndex index, indexList)
//        stringList.append(theSprints.sprint(index.row()).name());
//    for( int i = 0; i < theStoryTree.rowCount(); ++i )
//    {
//        if (checked) {
//            QString str = theStoryTree.data(i,ST_SPRINT).toString();
//            ui->storyTreeView->setRowHidden( i, !stringList.contains(str));
//        }
//        else
//            ui->storyTreeView->setRowHidden( i, false);
//    }
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


void MainWindow::setStories(const QVariantList &list)
{
    theStoryTree.fromList(list);
}

void MainWindow::addNewlySavedStory(const QVariantMap &map)
{
    // find parent and add child to model
    QModelIndex parentIndex = ui->storyTreeView->currentIndex();
    QModelIndex newIndex = theStoryTree.addStory(parentIndex, map);
//    ui->storyTreeView->setCurrentIndex(newIndex);
}

void MainWindow::setSprints(const QVariantList &list)
{
    theSprints.fromList(list);
    theSprints.sortByDate();
    QStandardItemModel *ms = static_cast<QStandardItemModel *>(ui->sprComboBox->model());
    for (int i=0; i<theSprints.rowCount();++i)
        ms->appendRow(new QStandardItem(theSprints.data(i, SP_NAME).toString()));
}


void setStandardItemModel(const QStringList &list, QAbstractItemModel *model)
{
    QStandardItemModel *ms = static_cast<QStandardItemModel *>(model);
    foreach (const QString &v, list)
        ms->appendRow(new QStandardItem(v));
}

void MainWindow::setPriorities(const QStringList &list)
{
    setStandardItemModel(list, ui->impComboBox->model());
}

void MainWindow::setEstimations(const QStringList &list)
{
    setStandardItemModel(list, ui->estComboBox->model());
}
void MainWindow::setVersions(const QStringList &list)
{
    setStandardItemModel(list, ui->verComboBox->model());
}
void MainWindow::setComponents(const QStringList &list)
{
    setStandardItemModel(list, ui->comComboBox->model());
}
void MainWindow::setTypes(const QStringList &list)
{
    setStandardItemModel(list, ui->typComboBox->model());
}

