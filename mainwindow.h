#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "views/burndownscene.h"
#include "views/storycardscene.h"
#include "models/storymodel.h"
#include "models/sprintmodel.h"
#include "io/iofactory.h"
#include "models/storyproxymodel.h"

#include <QMainWindow>
#include <QSettings>
#include <QPrinter>
#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QCheckBox>


namespace Ui {
    class MainWindow;
}

class AbstractStoryReport;

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
    void onActionReport(AbstractStoryReport &report, const QString &header);
    void on_actionShortReport_triggered();
    void onActionComprehensiveReport();
    void onActionMeetingHandout();
    void onActionPrint();
    //Story Table
    void onStoryTableCurrentCellChanged(const QModelIndex & , const QModelIndex & );
    void onStoryModelDataChanged(const QModelIndex &index);
    // Story Table Filters
    void applyStoryFilter();
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
    void setStatus(const QStringList &list);
    void setTypes(const QStringList &list);
    void loadError(const QString &);
    void ioMessage(const QString &);

    void updateStatisticsLabel(const QModelIndex &current);

private:
    //Helpers for story filter
    bool itemOrChildrenHaveDifferentStatus(const StoryItem *item, QString arg1);


private:
    Ui::MainWindow *ui;
    StoryModel theStoryTree;
    StoryProxyModel theStoryProxyTree;
    SprintModel theSprints;
    QStringList thePriorities;
    QStringList theEstimations;
    QStringList theVersions;
    QStringList theComponents;
    QStringList theStatus;
    QStringList theTypes;
    QDataWidgetMapper theStoryDataMapper;
    QDataWidgetMapper theSprintDataMapper;
    BurnDownScene theBurnDownScene;
    StoryCardScene theCardScene;
    QPrinter thePrinter;
    QSettings theSettings;
    bool loadOnStart;
    AbstractIO *theLoader;
    QMap<QString, QVariantMap> theStoryChanges;
};




#endif // MAINWINDOW_H
