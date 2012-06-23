#ifndef STORYREPORT_H
#define STORYREPORT_H

#include <QTextDocument>
#include <QTextCursor>


class StoryReport : public QTextDocument
{
    Q_OBJECT
public:
    explicit StoryReport(QObject *parent = 0);
    void insertheader(const QString &arg);
    void insertStory(int id, const QString &sum, const QString &desc, const QString &htd, const QString &prio, const QString &est, const QString &usr, const QString &stat);
    void beginInsertStory();
    void endInsertStory();
signals:
    
public slots:

private:
    QTextCursor theCursor;
    QMap <QString, QString> theMap;
};

#endif // STORYREPORT_H
