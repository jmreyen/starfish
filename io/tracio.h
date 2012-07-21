#ifndef TRACDATALOADER_H
#define TRACDATALOADER_H


#include "libmaia/maiaXmlRpcClient.h"
#include "abstractio.h"

class TracIO : public AbstractIO
{
    Q_OBJECT
public:
    explicit TracIO(QObject *parent = 0);
    bool load();
    bool saveNewStory(const QVariantMap &map);
    bool updateStories(QMap<QString, QVariantMap> &map);
    bool loadSettings(const QSettings &settings);
    bool saveSettings(QSettings &settings) const;

protected:
    void setUrl(const QUrl &url){theUrl = url; rpc.setUrl(url);}
    void setQueryString(const QString &s) {theQueryString=s;}
    const QString &queryString()const{return theQueryString;}
    const QUrl &url() const {return theUrl;}
    QString getStatus( QMap<QString,QVariant> &map) const;
    void parseSprintDescription(const QString &description, int &capacity, int &velocity, int &workDays, QList<int> &burnDown) const;

private:
    MaiaXmlRpcClient rpc;
    QUrl theUrl;
    QString theQueryString;

signals:
    
private slots:
    void ticketQueryResponseMethod(QVariant &arg);
    void sprintQueryResponseMethod(QVariant &arg);
    void componentQueryResponseMethod(QVariant &arg);
    void priorityQueryResponseMethod(QVariant &arg);
    void typeQueryResponseMethod(QVariant &arg);
    void versionQueryResponseMethod(QVariant &arg);
    void statusQueryResponseMethod(QVariant &arg);
    void estimationQueryResponseMethod(QVariant &arg);
    void getTicketResponseMethod(QVariant &arg);
    void getSprintResponseMethod(QVariant &arg);
    void saveNewStoryResponseMethod(QVariant &arg);
    void reloadNewStoryResponseMethod(QVariant &arg);
    void updateStoriesResponseMethod( QVariant &arg);
    void myFaultResponse(int error, const QString &message);
};

#endif // TRACDATALOADER_H
