#include "dialogs/newstorydialog.h"
#include "ui_newstorydialog.h"
#include "base/fields.h"
#include <QDebug>

NewStoryDialog::NewStoryDialog(QAbstractItemModel *m1,
                               QAbstractItemModel *m2,
                               QAbstractItemModel *m3,
                               QAbstractItemModel *m4,
                               QAbstractItemModel *m5,
                               QAbstractItemModel *m6,
                               const QString & parentId,
                               QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewStoryDialog),
    theParentId(parentId)
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
    QVariantMap newStory;
    newStory[storyFieldNames[ST_ID]]      = "";
    newStory[storyFieldNames[ST_DESC]]    = ui->summaryEdit->text();
    newStory[storyFieldNames[ST_NOTES]]   = ui->descriptionEdit->document()->toPlainText();
    newStory[storyFieldNames[ST_HTD]]     = ui->htdEdit->document()->toPlainText();
    newStory[storyFieldNames[ST_IMP]]     = ui->impComboBox->currentText();
    newStory[storyFieldNames[ST_EST]]     = ui->estComboBox->currentText();
    newStory[storyFieldNames[ST_USER]]    = ui->reporterEdit->text();
    newStory[storyFieldNames[ST_TYP]]     = ui->typComboBox->currentText();
    newStory[storyFieldNames[ST_SPRINT]]  = ui->sprComboBox->currentText();
    newStory[storyFieldNames[ST_COMP]]    = ui->comComboBox->currentText();
    newStory[storyFieldNames[ST_VERSION]] = ui->verComboBox->currentText();
    newStory[storyFieldNames[ST_STATUS]]  = "new";
    newStory[storyFieldNames[ST_PARENT]]  = theParentId;

    qDebug() << newStory << "\n";

    emit (accepted(newStory));
}
