#include "tracdataloader.h"

TracDataLoader::TracDataLoader(
        StoryModel  &sm,
        SprintModel &sp,
        QAbstractItemModel *pr,
        QAbstractItemModel *es,
        QAbstractItemModel *co,
        QAbstractItemModel *ve,
        QAbstractItemModel *ty,
        QAbstractItemModel *st,
        QObject *parent) :
AbstractDataLoader(sm, sp, pr, es, co, ve, ty, st, parent),
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

    rpc.call("ticket.status.getAll", args,
             this, SLOT(statusQueryResponseMethod(QVariant &)),
             this, SLOT(myFaultResponse(int, const QString &)));

//    rpc.call("ticket.estimation.getAll", args,
//             this, SLOT(estimationQueryResponseMethod(QVariant &)),
//             this, SLOT(myFaultResponse(int, const QString &)));

    QStringList estimations;
    estimations <<"?"<<"0"<<"0.5"<<"1"<<"2"<<"3"<<"5"<<"8"<<"13"<<"20"<<"40"<<"100";
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

void TracDataLoader::statusQueryResponseMethod(QVariant &arg)
{
    QStringList list = arg.toStringList();
    setStatus(list);
}

void TracDataLoader::estimationQueryResponseMethod(QVariant &arg)
{
    QStringList list = arg.toStringList();
    setEstimations(list);
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
    beginAddStory();
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
            map["status"].toString());
    }
    endAddStory();
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
//Feldnamen für TRAC. ACHTUNG: Die Reihenfolge muss die selbe sein wie in der Klasse StoryData (siehe storydata.h)
const char *fieldnames[] = {"id", "summary", "description", "how_to_demo", "priority", "estimation", "reporter", "type", "status", "milestone", "component", "version"};


void appendStoryArgs(const StoryData &d, QVariantList &args, int startPos)
{
    qDebug() << args;
}


bool TracDataLoader::saveNewStory(const StoryData &d)
{
    QVariantList args;
    QVariantMap arg;
    args.append(d[ST_DESC]);
    args.append(d[ST_NOTES]);
    for (int i=ST_HTD; i < ST_LAST; ++i) {
        arg.insert(fieldnames[i], d[i]);
    }
    args.append(arg);
    rpc.call("ticket.create", args,
                this, SLOT(saveNewStoryResponseMethod(QVariant &)),
                this, SLOT(myFaultResponse(int, const QString &)));
    return true;
}

void TracDataLoader::saveNewStoryResponseMethod(QVariant &arg)
{
    qDebug() << "New ID: " << arg.toInt() << "\n";
}


void TracDataLoader::myFaultResponse(int error, const QString &message) {
    QString msg = QString().sprintf("An Error occured: %i. Message: ", error) + message;
    qDebug() << msg;
//    ui->statusBar->showMessage(msg, 5000);
}

