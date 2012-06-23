#ifndef TRACDATALOADER_H
#define TRACDATALOADER_H


#include "libmaia/maiaXmlRpcClient.h"
#include "abstractdataloader.h"

class TracDataLoader : public AbstractDataLoader
{
    Q_OBJECT
public:
    explicit TracDataLoader(StoryModel  &st, SprintModel &sp, QObject *parent = 0);
    bool loadStories();
    bool loadSprints();
    void setUrl(const QUrl &url){theUrl = url; rpc.setUrl(url);}
    void setQueryString(const QString &s) {theQueryString=s;}
    const QString &queryString()const{return theQueryString;}
    const QUrl &url() const {return theUrl;}

protected:
    QString getStatus( QMap<QString,QVariant> &map) const;

private:
    MaiaXmlRpcClient rpc;
    QUrl theUrl;
    QString theQueryString;

signals:
    
public slots:
    void ticketQueryResponseMethod(QVariant &arg);
    void sprintQueryResponseMethod(QVariant &arg);
    void getTicketResponseMethod(QVariant &arg);
    void getSprintResponseMethod(QVariant &arg);
    void myFaultResponse(int error, const QString &message);
};

#endif // TRACDATALOADER_H
