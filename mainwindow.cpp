#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "storycardscene.h"


#include <QPrinter>
#include <QPrintDialog>
#include <QNetworkReply>
#include <QCheckBox>
#include <QMutex>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    theScene(new StoryCardScene(0,0,298,210,this)),
    thePrinter(new QPrinter(QPrinter::HighResolution))
{

    ui->setupUi(this);
    // setup card view
    ui->cardView->setScene(theScene);
    // setup Printer
    thePrinter->setOrientation(QPrinter::Landscape);
    thePrinter->setPageSize(QPrinter::A5);
    // initialise XMLRPC interface to trac
    //QUrl url("https://trac.f.ddk/dekafinanciallib/login/xmlrpc");
    QUrl url("http://admin:Millie007@localhost:8000/tracdata/login/xmlrpc");
    rpc = new MaiaXmlRpcClient(url, this);

    QTableWidgetItem* item = new QTableWidgetItem();
    item->setIcon(*(new QIcon("printer.png")));
    ui->storyTable->setHorizontalHeaderItem(8,item);



    ui->storyTable->setColumnHidden(2, true);
    ui->storyTable->setColumnHidden(3, true);
    ui->storyTable->setColumnHidden(4, true);
    ui->storyTable->setColumnHidden(5, true);
    ui->storyTable->setColumnHidden(6, true);
    ui->storyTable->setColumnHidden(7, true);

    ui->storyTable->setColumnWidth(0, 48);
    ui->storyTable->setColumnWidth(1, 256);
    ui->storyTable->setColumnWidth(8, 24);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete theScene;
    delete rpc;
    delete thePrinter;
}


void MainWindow::fillCard(int row, int col, StoryCardScene *scene)
{
    QString txt = ui->storyTable->item(row,col)?ui->storyTable->item(row,col)->text():"";

    if (scene==0x0)
        scene = theScene;

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
        scene = theScene;
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
    theScene->clearCard();
    ui->storyTable->removeRow(row);
}

void MainWindow::on_printButton_clicked()
{

    QPrintDialog printDialog(thePrinter);
    StoryCardScene scene;

    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(thePrinter);
        painter.setRenderHint(QPainter::Antialiasing);
        for (int i=0; i<ui->storyTable->rowCount(); ++i) {
            if (((QCheckBox*)ui->storyTable->cellWidget(i,8))->isChecked()) {
                fillCard(i, theScene);
                theScene->render(&painter);
                thePrinter->newPage();
            }
        }
    }
}

QMutex mutex;

void MainWindow::on_importButton_clicked()
{
    QVariantList args;
    args.append(QString("status!=closed&component^=DekaGrid"));

    rpc->call("ticket.query", args,
                this, SLOT(myResponseMethod(QVariant &)),
                this, SLOT(myFaultResponse(int, const QString &)));

    int rowcount = ui->storyTable->rowCount();
    for (int i = rowcount; i>=0; --i)
        ui->storyTable->removeRow(i);

}


void MainWindow::myResponseMethod(QVariant &arg) {
    QList<QVariant> list = arg.toList();
    for (int i = 0; i < list.size(); ++i) {
        QVariant item = list[i];
        QVariantList args;
        args.append(item.toInt());

        mutex.lock();
        rpc->call("ticket.get", args,
                    this, SLOT(myResponseMethod2(QVariant &)),
                    this, SLOT(myFaultResponse(int, const QString &)));

        mutex.unlock();
     }
}

void MainWindow::myResponseMethod2(QVariant &arg) {
    QList<QVariant> list = arg.toList();
    QMap<QString,QVariant> map = list[3].toMap();

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
         status = "not yet started";

    insertStoryRow(list[0].toInt(),
        map["summary"].toString(),
        map["description"].toString(),
        map["how_to_demo"].toString(),
        map["priority"].toString(),
        map["estimation"].toString(),
        map["reporter"].toString(),
        status);
}


void MainWindow::myFaultResponse(int error, const QString &message) {
    qDebug() << "An Error occoured, Code: " << error << " Message: " << message;
}

