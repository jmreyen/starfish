#ifndef TRACDATALOADER_H
#define TRACDATALOADER_H


#include "libmaia/maiaXmlRpcClient.h"
#include "abstractdataloader.h"

class TracDataLoader : public AbstractDataLoader
{
    Q_OBJECT
public:
    explicit TracDataLoader(StoryModel  &sm, SprintModel &sp, QAbstractItemModel *pr , QAbstractItemModel *es, QAbstractItemModel *co, QAbstractItemModel *ve,QAbstractItemModel *ty,QAbstractItemModel *st, QObject *parent);
    //loading data
    bool loadStories();
    bool loadSprints();
    bool loadMasterData();
    //storing data
    virtual bool saveNewStory(const StoryData &d);
    //setting up the Trac connection
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
    void componentQueryResponseMethod(QVariant &arg);
    void priorityQueryResponseMethod(QVariant &arg);
    void typeQueryResponseMethod(QVariant &arg);
    void versionQueryResponseMethod(QVariant &arg);
    void statusQueryResponseMethod(QVariant &arg);
    void estimationQueryResponseMethod(QVariant &arg);
    void getTicketResponseMethod(QVariant &arg);
    void getSprintResponseMethod(QVariant &arg);
    void saveNewStoryResponseMethod(QVariant &arg);
    void myFaultResponse(int error, const QString &message);
};

#endif // TRACDATALOADER_H
