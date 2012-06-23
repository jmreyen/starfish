#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "libmaia/maiaXmlRpcClient.h"
#include "burndownscene.h"
#include "storycardscene.h"
#include "storymodel.h"
#include "sprintmodel.h"

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
    void ticketQueryResponseMethod(QVariant &arg);
    void sprintQueryResponseMethod(QVariant &arg);
    void getTicketResponseMethod(QVariant &arg);
    void getSprintResponseMethod(QVariant &arg);
    void myFaultResponse(int error, const QString &message);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void applySettings();
    void fillCard(int row, int col, StoryCardScene *scene=0);
    void fillCard(int row, StoryCardScene *scene=0);
    void insertStoryRow(int id = -1, const QString &sum="", const QString &desc="", const QString &htd="", const QString &prio="?", const QString &est="?", const QString &usr="", const QString &ms="none", const QString &stat="new");
    QString getStatus( QMap<QString,QVariant> &map) const;
private slots:
    void onStoryTableCurrentCellChanged(const QModelIndex & , const QModelIndex & );
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
    MaiaXmlRpcClient rpc;
    QUrl theUrl;
    QSettings theSettings;
    QString theQueryString;
    bool loadOnStart;
};


#endif // MAINWINDOW_H
