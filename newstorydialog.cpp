#include "newstorydialog.h"
#include "ui_newstorydialog.h"
#include "storydata.h"

NewStoryDialog::NewStoryDialog(QAbstractItemModel *m1,
                               QAbstractItemModel *m2,
                               QAbstractItemModel *m3,
                               QAbstractItemModel *m4,
                               QAbstractItemModel *m5,
                               QAbstractItemModel *m6,
                               QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewStoryDialog)
{
    ui->setupUi(this);
    ui->estComboBox->setModel(m1);
    ui->impComboBox->setModel(m2);
    ui->sprComboBox->setModel(m3);
    ui->typComboBox->setModel(m4);
    ui->comComboBox->setModel(m5);
    ui->verComboBox->setModel(m6);
}

NewStoryDialog::~NewStoryDialog()
{
    delete ui;
}

void NewStoryDialog::on_buttonBox_accepted()
{
    QVariantMap newMap = StoryData::toMap(
            "",
            ui->summaryEdit->text(),
            ui->descriptionEdit->document()->toPlainText(),
            ui->htdEdit->document()->toPlainText(),
            ui->impComboBox->currentText(),
            ui->estComboBox->currentText(),
            ui->reporterEdit->text(),
            ui->typComboBox->currentText(),
            ui->sprComboBox->currentText(),
            ui->comComboBox->currentText(),
            ui->verComboBox->currentText(),
            "new");

    emit (accepted(newMap));
}
