#include "tracdataloader.h"

TracDataLoader::TracDataLoader(
        StoryModel  &st,
        SprintModel &sp,
        QAbstractItemModel *pr,
        QAbstractItemModel *es,
        QAbstractItemModel *co,
        QAbstractItemModel *ve,
        QAbstractItemModel *ty,
        QObject *parent) :
AbstractDataLoader(st, sp, pr, es, co, ve, ty, parent),
    rpc(this),
    theUrl()
{
}


bool TracDataLoader::loadStories()
{
    QVariantList ticketArgs;

    //Query Tickets
    ticketArgs.append(theQueryString+"&max=0");
    rpc.call("ticket.query", ticketArgs,
                this, SLOT(ticketQueryResponseMethod(QVariant &)),
                this, SLOT(myFaultResponse(int, const QString &)));

    return true;
}


bool TracDataLoader::loadSprints()
{
    QVariantList sprintArgs;

    //Query Milestones/Sprints
    rpc.call("ticket.milestone.getAll", sprintArgs,
                this, SLOT(sprintQueryResponseMethod(QVariant &)),
                this, SLOT(myFaultResponse(int, const QString &)));
    return true;
}

bool TracDataLoader::loadMasterData()
{
    QVariantList args;
    rpc.call("ticket.component.getAll", args,
             this, SLOT(componentQueryResponseMethod(QVariant &)),
             this, SLOT(myFaultResponse(int, const QString &)));

    rpc.call("ticket.priority.getAll", args,
             this, SLOT(priorityQueryResponseMethod(QVariant &)),
             this, SLOT(myFaultResponse(int, const QString &)));

    rpc.call("ticket.type.getAll", args,
             this, SLOT(typeQueryResponseMethod(QVariant &)),
             this, SLOT(myFaultResponse(int, const QString &)));

    rpc.call("ticket.version.getAll", args,
             this, SLOT(versionQueryResponseMethod(QVariant &)),
             this, SLOT(myFaultResponse(int, const QString &)));

    QStringList estimations;
    estimations << "?" << "1" << "3" << "5" << "8" << "13" << "20" << "40" << "100";
    setEstimations(estimations);
    return true;

}


void TracDataLoader::componentQueryResponseMethod(QVariant &arg)
{
    QStringList list = arg.toStringList();
    setComponents(list);
}

void TracDataLoader::priorityQueryResponseMethod(QVariant &arg)
{
    QStringList list = arg.toStringList();
    setPriorities(list);
}

void TracDataLoader::typeQueryResponseMethod(QVariant &arg)
{
    QStringList list = arg.toStringList();
    setTypes(list);
}

void TracDataLoader::versionQueryResponseMethod(QVariant &arg)
{
    QStringList list = arg.toStringList();
    setVersions(list);
}

void TracDataLoader::ticketQueryResponseMethod(QVariant &arg) {
    QStringList list = arg.toStringList();
    QVariantList args, methodList;

//    statusBar()->showMessage("Retrieving ...");
    for (int i=0; i<list.size(); ++i)    {
        QVariantMap newMethod;
        QVariantList newParams;
        newParams.append(list[i].toInt());
        newMethod.insert("methodName", "ticket.get");
        newMethod.insert("params", newParams);
        methodList.append(newMethod);
    }
    args.insert(0, methodList);;
    rpc.call("system.multicall", args, this, SLOT(getTicketResponseMethod(QVariant&)), this, SLOT(myFaultResponse(int, const QString &)));
}

void TracDataLoader::sprintQueryResponseMethod(QVariant &arg) {

    QStringList list = arg.toStringList();
    QVariantList args, methodList;

    for (int i=0; i<list.size(); ++i)    {
        QVariantMap newMethod;
        QVariantList newParams;
        newParams.append(list[i]);
        newMethod.insert("methodName", "ticket.milestone.get");
        newMethod.insert("params", newParams);
        methodList.append(newMethod);
    }
    args.insert(0, methodList);;
    rpc.call("system.multicall", args, this, SLOT(getSprintResponseMethod(QVariant&)), this, SLOT(myFaultResponse(int, const QString &)));
}

void TracDataLoader::getTicketResponseMethod(QVariant &arg)
{
    QVariantList ticketList = arg.toList();
    for (int i = 0; i < ticketList.size(); ++i) {
        QVariantList fieldList = ticketList[i].toList().at(0).toList();
        QMap<QString,QVariant> map = fieldList[3].toMap();
        addStory(
            fieldList[0].toInt(),
            map["summary"].toString(),
            map["description"].toString(),
            map["how_to_demo"].toString(),
            map["priority"].toString(),
            map["estimation"].toString(),
            map["reporter"].toString(),
            map["type"].toString(),
            map["milestone"].toString(),
            map["component"].toString(),
            map["version"].toString(),
            getStatus(map));
    }
//    statusBar()->showMessage("");
}

void TracDataLoader::getSprintResponseMethod(QVariant &arg)
{
    QVariantList sprintList = arg.toList();
    for (int i = 0; i < sprintList.size(); ++i) {
        QMap<QString, QVariant> map = sprintList[i].toList().at(0).toMap();
        QString nam = map["name"].toString();
        QDate dat = map["due"].toDateTime().date();
        bool cmp = map["completed"].toString()!= "0";
        QString dsc = map["description"].toString();
        addSprint(nam, dat, cmp, dsc);
    }
}



QString TracDataLoader::getStatus( QMap<QString,QVariant> &map) const {

    QString status = "unknown";
    if (map["status"].toString().left(8) == QString("new"))
        status = "new";
    else if (map["milestone"]==QString("waiting"))
        status = "waiting";
    else if (map["status"].toString().left(8) == QString("postponed"))
        status = "postponed";
    else if (map["milestone"]==QString("current"))
        status = "selected";
    else if (map["status"].toString().left(8) == QString("closed"))
        status = "done";
    else if ((map["status"]==QString("assigned") ||
              map["status"]==QString("accepted")) &&
             (map["milestone"]==QString("none") ||
              map["milestone"]==QString("")))
         status = "backlog";

    return status;
}


void TracDataLoader::myFaultResponse(int error, const QString &message) {
    QString msg = QString().sprintf("An Error occured: %i. Message: ", error) + message;
    qDebug() << msg;
//    ui->statusBar->showMessage(msg, 5000);
}

