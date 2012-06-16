#ifndef TICKETREPORT_H
#define TICKETREPORT_H

#include <QTextDocument>
#include <QTextCursor>


class TicketReport : public QTextDocument
{
    Q_OBJECT
public:
    explicit TicketReport(QObject *parent = 0);
    void insertheader(const QString &arg);
    void insertTicket(int id, const QString &sum, const QString &desc, const QString &htd, const QString &prio, const QString &est, const QString &usr, const QString &stat);
    void beginInsertTicket();
    void endInsertTicket();
signals:
    
public slots:

private:
    QTextCursor theCursor;
    QMap <QString, QString> theMap;
};

#endif // TICKETREPORT_H
