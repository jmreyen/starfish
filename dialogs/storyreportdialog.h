#ifndef REPORTDIALOG_H
#define REPORTDIALOG_H

#include <QDialog>
#include <QTextDocument>

namespace Ui {
class ReportDialog;
}

class ReportDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ReportDialog(QWidget *parent = 0);
    ~ReportDialog();
    void setHtml(const QString &s);
    
private slots:
    void on_printButton_clicked();

private:
    Ui::ReportDialog *ui;
};

#endif // REPORTDIALOG_H
