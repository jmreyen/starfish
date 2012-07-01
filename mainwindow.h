#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "burndownscene.h"
#include "storycardscene.h"
#include "storymodel.h"
#include "sprintmodel.h"
#include "tracdataloader.h"

#include <QMainWindow>
#include <QSettings>
#include <QPrinter>
#include <QStandardItemModel>
#include <QDataWidgetMapper>

class QPrinter;
class QGraphicsTextItem;
class StoryCardScene;

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
    void loadAll();
    void fillCard(int row, int col, StoryCardScene *scene=0);
    void fillCard(int row, StoryCardScene *scene=0);
private slots:
    void onStoryTableCurrentCellChanged(const QModelIndex & , const QModelIndex & );
    void onStoryModelDataChanged(const QModelIndex &index);
    void onSprintTableCurrentCellChanged(const QModelIndex & , const QModelIndex & );
    void onSprintModelDataChanged(const QModelIndex &index);
    void onStoryTableLayoutAboutToBeChanged();
    void onStoryTableLayoutChanged();
    void on_addRowButton_clicked();
    void onNewStoryAccepted(QVariantMap map);
    void on_printButton_clicked();
    void on_importButton_clicked();
    void on_setupButton_clicked();
    void onSetupAccepted(QVariantMap);
    void onFilterRow(QString arg);
    void on_reportButton_clicked();
    void on_filterBySprintCheckBox_clicked(bool checked);

    void on_saveStoryButton_clicked();

private:
    Ui::MainWindow *ui;
    StoryModel theStories;
    SprintModel theSprints;
    QDataWidgetMapper theStoryDataMapper;
    QDataWidgetMapper theSprintDataMapper;
    BurnDownScene theBurnDownScene;
    StoryCardScene theCardScene;
    QPrinter thePrinter;
    QSettings theSettings;
    bool loadOnStart;
    TracDataLoader *theLoader;
    QList<QPersistentModelIndex> theStoryChanges;
};


#endif // MAINWINDOW_H
