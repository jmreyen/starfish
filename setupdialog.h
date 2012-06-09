#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>
#include <QVariant>

namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SetupDialog(QWidget *parent = 0);
    ~SetupDialog();
    void setHideColumn(int col, bool hide);
signals:
    void accepted(QVariantList q);
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::SetupDialog *ui;
};

#endif // SETUPDIALOG_H
