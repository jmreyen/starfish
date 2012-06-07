#include "setupdialog.h"
#include "ui_setupdialog.h"

SetupDialog::SetupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupDialog)
{
    ui->setupUi(this);
}

SetupDialog::~SetupDialog()
{
    delete ui;
}

void SetupDialog::on_buttonBox_accepted()
{

}

bool SetupDialog::isColumnHidden(int i)
{
    switch (i){
        case 1: return ui->checkBox_1->isChecked();
        case 2: return ui->checkBox_2->isChecked();
        case 3: return ui->checkBox_3->isChecked();
        case 4: return ui->checkBox_4->isChecked();
        case 5: return ui->checkBox_5->isChecked();
        case 6: return ui->checkBox_6->isChecked();
        case 7: return ui->checkBox_7->isChecked();
        default: return false;
    }
}
