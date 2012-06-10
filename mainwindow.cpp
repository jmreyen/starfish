#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "setupdialog.h"

#include <QPrinter>
#include <QPrintDialog>
#include <QNetworkReply>
#include <QCheckBox>
#include <QMutex>
#include <sstream>

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

    // setup card view
    ui->cardView->setScene(&theScene);

    // Load Settings
    //TRAC
    theUrl.setUrl(theSettings.value("TRAC/Server").toString());
    theUrl.setPort(theSettings.value("TRAC/Port").toInt());
    theUrl.setUserName(theSettings.value("TRAC/UserName").toString());
    theUrl.setPassword(theSettings.value("TRAC/Password").toString());
    rpc.setUrl(theUrl);
    theQueryString = theSettings.value("TRAC/QueryString").toString();

    //Columns
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
}

MainWindow::~MainWindow()
{
    // Save Settings
    //TRAC
    theSettings.setValue("TRAC/Server", theUrl.toString(QUrl::RemoveUserInfo|QUrl::RemovePort));
    theSettings.setValue("TRAC/UserName", theUrl.userName());
    theSettings.setValue("TRAC/Password", theUrl.password());
    theSettings.setValue("TRAC/QueryString", theQueryString);

    // Columns
    theSettings.beginWriteArray("columns");
    for (int i=0; i<ui->storyTable->columnCount(); ++i){
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
    QString txt = ui->storyTable->item(row,col)?ui->storyTable->item(row,col)->text():"";

    if (scene==0x0)
        scene = &theScene;

    if (ui->storyTable->rowCount()==0 || row == -1)
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


void MainWindow::insertStoryRow(int id, const QString &sum, const QString &desc, const QString &htd, const QString &prio, const QString &est, const QString &usr, const QString &stat)
{
    int row = ui->storyTable->rowCount();

    ui->storyTable->insertRow(row);
    QTableWidgetItem *item = new QTableWidgetItem();
    if (id>=0)
        item->setData(Qt::DisplayRole, id);
    else
        item->setText("");
    item->setTextAlignment(Qt::AlignRight);
    ui->storyTable->setItem(row,0, item);
    ui->storyTable->item(row,0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    ui->storyTable->setItem(row,1, new QTableWidgetItem(sum));
    ui->storyTable->setItem(row,2, new QTableWidgetItem(desc));
    ui->storyTable->setItem(row,3, new QTableWidgetItem(htd));
    ui->storyTable->setItem(row,4, new QTableWidgetItem(prio));
    ui->storyTable->setItem(row,5, new QTableWidgetItem(est));
    ui->storyTable->setItem(row,6, new QTableWidgetItem(usr));
    ui->storyTable->setItem(row,7, new QTableWidgetItem(stat));
    //ui->storyTable->setItem(row,8, new QTableWidgetItem(""));
    ui->storyTable->setCellWidget(row, 8, new QCheckBox());

}

//void MainWindow::setColumnHidden(int col, bool hide)
//{
//    ui->storyTable->setColumnHidden(col, hide);
//}

void MainWindow::on_storyTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if (currentRow!=previousRow) {
        fillCard(currentRow);
        ui->cardView->show();
    }
}

void MainWindow::on_storyTable_cellChanged(int row, int column)
{
    fillCard(row, column);
    ui->cardView->show();
}

void MainWindow::on_addRowButton_clicked()
{
    insertStoryRow();
}

void MainWindow::on_removeRowButton_clicked()
{
    int row = ui->storyTable->currentRow();
    theScene.clearCard();
    ui->storyTable->removeRow(row);
}

void MainWindow::on_printButton_clicked()
{

    QPrintDialog printDialog(&thePrinter);
    StoryCardScene scene;

    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&thePrinter);
        painter.setRenderHint(QPainter::Antialiasing);
        for (int i=0; i<ui->storyTable->rowCount(); ++i) {
            if (((QCheckBox*)ui->storyTable->cellWidget(i,8))->isChecked()) {
                fillCard(i, &theScene);
                theScene.render(&painter);
                thePrinter.newPage();
            }
        }
    }
}

QMutex mutex;

void MainWindow::on_importButton_clicked()
{
    QVariantList args;

    statusBar()->showMessage("Querying ...");
    args.append(theQueryString);
    rpc.call("ticket.query", args,
                this, SLOT(queryResponseMethod(QVariant &)),
                this, SLOT(myFaultResponse(int, const QString &)));

    int rowcount = ui->storyTable->rowCount();
    for (int i = rowcount; i>=0; --i)
        ui->storyTable->removeRow(i);

}


void MainWindow::queryResponseMethod(QVariant &arg) {
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
    rpc.call("system.multicall", args, this, SLOT(insertResponseMethod(QVariant&)), this, SLOT(myFaultResponse(int, const QString &)));
}

void MainWindow::insertResponseMethod(QVariant &arg)
{
    QVariantList ticketList = arg.toList();
    for (int i = 0; i < ticketList.size(); ++i) {
        QVariantList fieldList = ticketList[i].toList().at(0).toList();
        QMap<QString,QVariant> map = fieldList[3].toMap();
            insertStoryRow(fieldList[0].toInt(),
                map["summary"].toString(),
                map["description"].toString(),
                map["how_to_demo"].toString(),
                map["priority"].toString(),
                map["estimation"].toString(),
                map["reporter"].toString(),
                           getStatus(map));
    }
    statusBar()->showMessage("");
}


QString MainWindow::getStatus( QMap<QString,QVariant> &map) const {

    QString status = "unkown";
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
    for (int i=0; i<ui->storyTable->columnCount(); ++i)
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
