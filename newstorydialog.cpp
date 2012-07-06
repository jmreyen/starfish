#include "newstorydialog.h"
#include "ui_newstorydialog.h"
#include "storydata.h"

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
    QVariantMap newMap = StoryData::toMap(
            "",
            ui->summaryEdit->text(),
            ui->descriptionEdit->document()->toPlainText(),
            ui->htdEdit->document()->toPlainText(),
            ui->impComboBox->currentText(),
            ui->estComboBox->currentText(),
            ui->reporterEdit->text(),
            ui->typComboBox->currentText(),
            "new",
            ui->sprComboBox->currentText(),
            ui->comComboBox->currentText(),
            ui->verComboBox->currentText());

    emit (accepted(newMap));
}
