#ifndef NEWSTORYDIALOG_H
#define NEWSTORYDIALOG_H

#include <QDialog>

namespace Ui {
class NewStoryDialog;
}

class NewStoryDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewStoryDialog(QWidget *parent = 0);
    ~NewStoryDialog();
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewStoryDialog *ui;
};

#endif // NEWSTORYDIALOG_H
