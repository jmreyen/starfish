#include "setupdialog.h"
#include "ui_setupdialog.h"

#include <QUrl>


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

void SetupDialog::setUrl(const QUrl& url)
{
    ui->serverEdit->setText(url.toString(QUrl::RemoveUserInfo|QUrl::RemovePort));
    ui->portSpinBox->setValue(url.port());
    ui->userEdit->setText(url.userName());
    ui->passwordEdit->setText(url.password());
}

void SetupDialog::setQueryString(const QString &str)
{
    ui->queryGeneralEdit->setText(str);
}


void SetupDialog::setShowColumn(int col, bool b)
{
    switch (col)
    {
    case 0: ui->checkBox_0->setChecked(b); break;
    case 1: ui->checkBox_1->setChecked(b); break;
    case 2: ui->checkBox_2->setChecked(b); break;
    case 3: ui->checkBox_3->setChecked(b); break;
    case 4: ui->checkBox_4->setChecked(b); break;
    case 5: ui->checkBox_5->setChecked(b); break;
    case 6: ui->checkBox_6->setChecked(b); break;
    case 7: ui->checkBox_7->setChecked(b); break;
    case 8: ui->checkBox_8->setChecked(b); break;
    }
}

void SetupDialog::setLoadOnStart(bool b)
{
    ui->loadOnStartCheckBox->setChecked(b);
}

void SetupDialog::on_buttonBox_accepted()
{
    QVariantMap returnMap;
    QVariantList columnlist;
    QUrl url;

    url.setUrl(ui->serverEdit->text());
    url.setPort(ui->portSpinBox->value());
    url.setUserName(ui->userEdit->text());
    url.setPassword(ui->passwordEdit->text());
    returnMap["Url"] = url;

    returnMap["QueryString"] = ui->queryGeneralEdit->text();

    columnlist << ui->checkBox_0->isChecked() << ui->checkBox_1->isChecked()<<
            ui->checkBox_2->isChecked() << ui->checkBox_3->isChecked() <<
            ui->checkBox_4->isChecked() << ui->checkBox_5->isChecked() <<
            ui->checkBox_6->isChecked() << ui->checkBox_7->isChecked()<<
            ui->checkBox_8->isChecked();
    returnMap["Columns"] = columnlist;
    returnMap["LoadOnStart"] = ui->loadOnStartCheckBox->isChecked();

    emit accepted(returnMap);
}
