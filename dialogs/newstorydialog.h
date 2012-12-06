#ifndef NEWSTORYDIALOG_H
#define NEWSTORYDIALOG_H

#include <QDialog>
#include <QVariant>

class QAbstractItemModel;

namespace Ui {
class NewStoryDialog;
}

class NewStoryDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewStoryDialog(QAbstractItemModel *m1,
                            QAbstractItemModel *m2,
                            QAbstractItemModel *m3,
                            QAbstractItemModel *m4,
                            QAbstractItemModel *m5,
                            QAbstractItemModel *m6,
                            const QString &parentId, QWidget *parent = 0);

    ~NewStoryDialog();

signals:
    void accepted(const QVariantMap &map);
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewStoryDialog *ui;
    QString theParentId;
};

#endif // NEWSTORYDIALOG_H
