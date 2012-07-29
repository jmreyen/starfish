#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "burndownscene.h"
#include "storycardscene.h"
#include "storyitemmodel.h"
#include "sprintmodel.h"
#include "io/iofactory.h"

#include <QMainWindow>
#include <QSettings>
#include <QPrinter>
#include <QStandardItemModel>
#include <QDataWidgetMapper>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public slots:

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    // Load/Store
    void loadAll();
    //Display Index Card
    void fillCard(const QModelIndex &index, StoryCardScene *scene=0);
private slots:
    //Toolbar Actions
    void onActionAddStory();
    void onActionStoreStories();
    void onActionLoad();
    void onActionSettings();
    void onSetupAccepted(const QVariantMap &map);
    void onActionReport();
    void onActionPrint();
    //Story Table
    void onStoryTableCurrentCellChanged(const QModelIndex & , const QModelIndex & );
    void onStoryModelDataChanged(const QModelIndex &index);
    void onStoryTableLayoutAboutToBeChanged() const;
    void onStoryTableLayoutChanged();
    // Story Table Filters
    void onFilterRow(QString arg);
    void on_filterBySprintCheckBox_clicked(bool checked);
    //Sprint Table
    void onSprintTableCurrentCellChanged(const QModelIndex & , const QModelIndex & );
    void onSprintModelDataChanged(const QModelIndex &index);
    //Data Loader Response Methods
    void setStories(const QVariantList &list);
    void addNewlySavedStory(const QVariantMap &map);
    void setSprints(const QVariantList &list);
    void setPriorities(const QStringList &list);
    void setEstimations(const QStringList &list);
    void setVersions(const QStringList &list);
    void setComponents(const QStringList &list);
    void setTypes(const QStringList &list);


private:
    Ui::MainWindow *ui;
    StoryItemModel theStoryTree;
    SprintModel theSprints;
    QDataWidgetMapper theStoryDataMapper;
    QDataWidgetMapper theSprintDataMapper;
    BurnDownScene theBurnDownScene;
    StoryCardScene theCardScene;
    QPrinter thePrinter;
    QSettings theSettings;
    bool loadOnStart;
    AbstractIO *theLoader;
    QList<QPersistentModelIndex> theStoryChanges;
};


#endif // MAINWINDOW_H
