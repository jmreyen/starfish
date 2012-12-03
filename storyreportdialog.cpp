#include "storyreportdialog.h"
#include "ui_reportdialog.h"

#include <QPrinter>
#include <QPrintDialog>

ReportDialog::ReportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportDialog)
{
    ui->setupUi(this);
}

ReportDialog::~ReportDialog()
{
    delete ui;
}

void ReportDialog::setHtml(const QString &s)
{
    ui->webView->setHtml(s);
}

void ReportDialog::on_printButton_clicked()
{
    QPrinter printer;
    printer.setOrientation(QPrinter::Portrait);
    printer.setPageSize(QPrinter::A4);
    QPrintDialog dlg(&printer);
    dlg.exec();
    ui->webView->print(&printer);
}
