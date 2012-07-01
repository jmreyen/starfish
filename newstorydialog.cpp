#include "newstorydialog.h"
#include "ui_newstorydialog.h"

NewStoryDialog::NewStoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewStoryDialog)
{
    ui->setupUi(this);
}

NewStoryDialog::~NewStoryDialog()
{
    delete ui;
}

void NewStoryDialog::on_buttonBox_accepted()
{

}
