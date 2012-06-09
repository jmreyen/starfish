#include "setupdialog.h"
#include "ui_setupdialog.h"

#include <QUrl>

class urlValidator : public QValidator
{
public:
    void fixup ( QString & input ) const {}
    QValidator::State validate(QString &input, int &pos) const {return QUrl(input).isValid()?QValidator::Acceptable:QValidator::Intermediate;}
};

SetupDialog::SetupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setValidator(new urlValidator);
}

SetupDialog::~SetupDialog()
{
    delete ui;
}
void SetupDialog::setHideColumn(int col, bool hide)
{
    switch (col)
    {
    case 0: ui->checkBox_0->setChecked(hide); break;
    case 1: ui->checkBox_1->setChecked(hide); break;
    case 2: ui->checkBox_2->setChecked(hide); break;
    case 3: ui->checkBox_3->setChecked(hide); break;
    case 4: ui->checkBox_4->setChecked(hide); break;
    case 5: ui->checkBox_5->setChecked(hide); break;
    case 6: ui->checkBox_6->setChecked(hide); break;
    case 7: ui->checkBox_7->setChecked(hide); break;
    case 8: ui->checkBox_8->setChecked(hide); break;
    }
}

void SetupDialog::on_buttonBox_accepted()
{
    QVariantList list;
    list << ui->checkBox_0->isChecked() << ui->checkBox_1->isChecked()<<
            ui->checkBox_2->isChecked() << ui->checkBox_3->isChecked() <<
            ui->checkBox_4->isChecked() << ui->checkBox_5->isChecked() <<
            ui->checkBox_6->isChecked() << ui->checkBox_7->isChecked()<<
            ui->checkBox_8->isChecked();

    emit accepted(list);
}
