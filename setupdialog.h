#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>

namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SetupDialog(QWidget *parent = 0);
    ~SetupDialog();
    bool isColumnHidden(int i);
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::SetupDialog *ui;
};

#endif // SETUPDIALOG_H
