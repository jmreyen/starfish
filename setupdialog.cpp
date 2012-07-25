#include <QUrl>

#include "ui_setupdialog.h"
#include "setupdialog.h"
#include "fields.h"


SetupDialog::SetupDialog(QFrame *extraSettings, QWidget *parent):
    QDialog(parent),
    ui(new Ui::SetupDialog)
{
    ui->setupUi(this);
    ui->tabWidget->addTab(extraSettings, extraSettings->objectName());
}

SetupDialog::~SetupDialog()
{
    delete ui;
}



void SetupDialog::setShowColumn(int col, bool b)
{
    switch (col)
    {
    case ST_ID:      ui->checkBox_0->setChecked(b); break;
    case ST_DESC:    ui->checkBox_1->setChecked(b); break;
    case ST_NOTES:   ui->checkBox_2->setChecked(b); break;
    case ST_HTD:     ui->checkBox_3->setChecked(b); break;
    case ST_IMP:     ui->checkBox_4->setChecked(b); break;
    case ST_EST:     ui->checkBox_5->setChecked(b); break;
    case ST_USER:    ui->checkBox_6->setChecked(b); break;
    case ST_TYP:     ui->checkBox_7->setChecked(b); break;
    case ST_STATUS:  ui->checkBox_8->setChecked(b); break;
    case ST_SPRINT:  ui->checkBox_9->setChecked(b); break;
    case ST_COMP:    ui->checkBox_10->setChecked(b); break;
    case ST_VERSION: ui->checkBox_11->setChecked(b); break;
    case ST_PARENT:  ui->checkBox_12->setChecked(b); break;
    case ST_LAST:    ui->checkBox_13->setChecked(b); break;
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

    columnlist << ui->checkBox_0->isChecked() << ui->checkBox_1->isChecked()<<
            ui->checkBox_2->isChecked() << ui->checkBox_3->isChecked() <<
            ui->checkBox_4->isChecked() << ui->checkBox_5->isChecked() <<
            ui->checkBox_6->isChecked() << ui->checkBox_7->isChecked()<<
            ui->checkBox_8->isChecked() << ui->checkBox_9->isChecked()<<
            ui->checkBox_10->isChecked()<< ui->checkBox_11->isChecked()<<
            ui->checkBox_12->isChecked()<< ui->checkBox_13->isChecked();
    returnMap["Columns"] = columnlist;
    returnMap["LoadOnStart"] = ui->loadOnStartCheckBox->isChecked();

    emit accepted(returnMap);
}
