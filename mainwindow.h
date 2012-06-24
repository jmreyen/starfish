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
    void applySettings();
    void fillCard(int row, int col, StoryCardScene *scene=0);
    void fillCard(int row, StoryCardScene *scene=0);
    void insertStoryRow(int id = -1, const QString &sum="", const QString &desc="", const QString &htd="", const QString &prio="?", const QString &est="?", const QString &usr="", const QString &typ="", const QString &ms="none", const QString &stat="new");
private slots:
    void onStoryTableCurrentCellChanged(const QModelIndex & , const QModelIndex & );
    void onStoryModelDataChanged(const QModelIndex &index);
    void onSprintTableCurrentCellChanged(const QModelIndex & , const QModelIndex & );
    void onSprintModelDataChanged(const QModelIndex &index);
    void on_addRowButton_clicked();
    void on_removeRowButton_clicked();
    void on_printButton_clicked();
    void on_importButton_clicked();
    void on_setupButton_clicked();
    void onSetupAccepted(QVariantMap);
    void onFilterRow(QString arg);
    void on_reportButton_clicked();
    void on_filterBySprintCheckBox_clicked(bool checked);

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
};


#endif // MAINWINDOW_H
