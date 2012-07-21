#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>
#include <QVariant>
#include <QFrame>

namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SetupDialog(QFrame *extraSettings, QWidget *parent = 0);
    ~SetupDialog();
    void setShowColumn(int col, bool b);
    void setLoadOnStart(bool b);
signals:
    void accepted(const QVariantMap &map);
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::SetupDialog *ui;
};

#endif // SETUPDIALOG_H
