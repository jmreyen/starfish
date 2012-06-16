#include "ticketreportdialog.h"
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

void ReportDialog::setTextDocument(QTextDocument *d)
{
    ui->reportEdit->setDocument(d);
}

void ReportDialog::on_printButton_clicked()
{
    QPrinter printer;
    QPrintDialog dlg(&printer);
    dlg.exec();
    ui->reportEdit->print(&printer);
}
