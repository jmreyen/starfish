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
    void setUrl(const QUrl& url);
    void setShowColumn(int col, bool b);
    void setQueryString(const QString &str);
    void setLoadOnStart(bool b);
signals:
    void accepted(const QVariantMap &map);
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::SetupDialog *ui;
};

#endif // SETUPDIALOG_H
