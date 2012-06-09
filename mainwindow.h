#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "libmaia/maiaXmlRpcClient.h"
#include <QMainWindow>
#include <QSettings>

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
    void queryResponseMethod(QVariant &arg);
    void insertResponseMethod(QVariant &arg);
    void myFaultResponse(int error, const QString &message);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void applySettings();
    void fillCard(int row, int col, StoryCardScene *scene=0);
    void fillCard(int row, StoryCardScene *scene=0);
    void insertStoryRow(int id = -1, const QString &sum="", const QString &desc="", const QString &htd="", const QString &prio="?", const QString &est="?", const QString &usr="", const QString &stat="new");

private slots:
    void on_storyTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_storyTable_cellChanged(int row, int column);
    void on_addRowButton_clicked();
    void on_removeRowButton_clicked();
    void on_printButton_clicked();
    void on_importButton_clicked();
    void on_setupButton_clicked();
    void onSetupAccepted(QVariantList);

private:
    Ui::MainWindow *ui;
    StoryCardScene *theScene;
    MaiaXmlRpcClient *rpc;
    QPrinter *thePrinter;
    QSettings settings;
};


#endif // MAINWINDOW_H
