#include "tracdataloader.h"
#include "sprintmodel.h"
#include "storyitem.h"

TracDataLoader::TracDataLoader(
        QObject *parent) :
AbstractDataLoader(parent),
    rpc(this),
    theUrl()
{
}


bool TracDataLoader::saveNewStory(const QVariantMap &map)
{
    QVariantList args;
    QVariantMap attributes;
    args.append(map[storyFieldNames[ST_DESC]]);
    args.append(map[storyFieldNames[ST_NOTES]]);
    for (int i=ST_HTD; i < ST_LAST; ++i) {
        attributes.insert(storyFieldNames[i], map[storyFieldNames[i]]);
    }
    args.append(attributes);
    rpc.call("ticket.create", args,
                this, SLOT(saveNewStoryResponseMethod(QVariant &)),
                this, SLOT(myFaultResponse(int, const QString &)));
    return true;
}



bool TracDataLoader::updateStories(QMap<QString, QVariantMap> & map)
{
    QVariantList args, methodList;

    for (QMap<QString, QVariantMap>::iterator itr = map.begin(); itr != map.end(); ++itr) {

        QVariantMap newMethod;
        QVariantList newParams;
        newParams.append(itr.key().toInt());
        newParams.append("Stored by PIC");
        newParams.append(itr.value());
        newMethod.insert("methodName", "ticket.update");
        newMethod.insert("params", newParams);
        methodList.append(newMethod);
    }

    args.insert(0, methodList);;
    rpc.call("system.multicall", args,
             this, SLOT(updateStoriesResponseMethod(QVariant&)),
             this, SLOT(myFaultResponse(int, const QString &)));
    return true;
}



bool TracDataLoader::load()
{
    QVariantList ticketArgs;
    QVariantList sprintArgs;
    QVariantList emptyArgs;


    //Query Tickets
    ticketArgs.append(theQueryString+"&max=0");
    rpc.call("ticket.query", ticketArgs,
                this, SLOT(ticketQueryResponseMethod(QVariant &)),
                this, SLOT(myFaultResponse(int, const QString &)));

    //Query Milestones/Sprints
    rpc.call("ticket.milestone.getAll", sprintArgs,
                this, SLOT(sprintQueryResponseMethod(QVariant &)),
                this, SLOT(myFaultResponse(int, const QString &)));

    rpc.call("ticket.component.getAll", emptyArgs,
             this, SLOT(componentQueryResponseMethod(QVariant &)),
             this, SLOT(myFaultResponse(int, const QString &)));

    rpc.call("ticket.priority.getAll", emptyArgs,
             this, SLOT(priorityQueryResponseMethod(QVariant &)),
             this, SLOT(myFaultResponse(int, const QString &)));

    rpc.call("ticket.type.getAll", emptyArgs,
             this, SLOT(typeQueryResponseMethod(QVariant &)),
             this, SLOT(myFaultResponse(int, const QString &)));

    rpc.call("ticket.version.getAll", emptyArgs,
             this, SLOT(versionQueryResponseMethod(QVariant &)),
             this, SLOT(myFaultResponse(int, const QString &)));

    rpc.call("ticket.status.getAll", emptyArgs,
             this, SLOT(statusQueryResponseMethod(QVariant &)),
             this, SLOT(myFaultResponse(int, const QString &)));

//    rpc.call("ticket.estimation.getAll", args,
//             this, SLOT(estimationQueryResponseMethod(QVariant &)),
//             this, SLOT(myFaultResponse(int, const QString &)));

    QVariantList estimations;
    estimations <<"?"<<"0"<<"0.5"<<"1"<<"2"<<"3"<<"5"<<"8"<<"13"<<"20"<<"40"<<"100";
    QVariant estVariant = estimations;
    estimationQueryResponseMethod(estVariant);

    return true;
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
    rpc.call("system.multicall", args,
             this, SLOT(getTicketResponseMethod(QVariant&)),
             this, SLOT(myFaultResponse(int, const QString &)));
}

void TracDataLoader::getTicketResponseMethod(QVariant &arg)
{
    QVariantList ticketList = arg.toList();
    QVariantList storyList;
    for (int i = 0; i < ticketList.size(); ++i) {
        QVariantList fieldList = ticketList[i].toList().at(0).toList();
        QMap<QString,QVariant> map = fieldList[3].toMap();
        QVariantMap newStory;
        newStory[storyFieldNames[ST_ID]]      = fieldList[0].toString();
        newStory[storyFieldNames[ST_DESC]]    = map["summary"].toString();
        newStory[storyFieldNames[ST_NOTES]]   = map["description"].toString();
        newStory[storyFieldNames[ST_HTD]]     = map["how_to_demo"].toString();
        newStory[storyFieldNames[ST_IMP]]     = map["priority"].toString();
        newStory[storyFieldNames[ST_EST]]     = map["estimation"].toString();
        newStory[storyFieldNames[ST_USER]]    = map["reporter"].toString();
        newStory[storyFieldNames[ST_TYP]]     = map["type"].toString();
        newStory[storyFieldNames[ST_STATUS]]  = map["milestone"].toString();
        newStory[storyFieldNames[ST_SPRINT]]  = map["component"].toString();
        newStory[storyFieldNames[ST_COMP]]    = map["version"].toString();
        newStory[storyFieldNames[ST_VERSION]] = map["status"].toString();
        storyList.append(newStory);
    }
    emit storiesLoaded(storyList);
//    statusBar()->showMessage("");
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
    rpc.call("system.multicall", args,
             this, SLOT(getSprintResponseMethod(QVariant&)),
             this, SLOT(myFaultResponse(int, const QString &)));
}


void TracDataLoader::getSprintResponseMethod(QVariant &arg)
{
    QVariantList sprintList = arg.toList();
    QVariantList newList;
    for (int i = 0; i < sprintList.size(); ++i) {
        QMap<QString, QVariant> map = sprintList[i].toList().at(0).toMap();
        int capacity = 0, velocity = 0, workdays = 0;
        QString description;
        QList<int> burnDown;
        parseSprintDescription(map["description"].toString(), capacity, velocity, workdays, burnDown);
        QVariantMap newMap = SprintData::toMap(
                    map["name"].toString(),
                    map["due"].toDateTime().date(),
                    map["completed"].toString()!= "0",
                    map["description"].toString(),
                    capacity,
                    velocity,
                    workdays,
                    burnDown);
        newList.append(newMap);
    }
    emit sprintsLoaded(newList);
}

void TracDataLoader::parseSprintDescription(const QString &description, int &capacity, int &velocity, int &workDays, QList<int> &burnDown) const
{
    capacity = 0;
    velocity = 0;
    workDays = 0;

    QRegExp rx("burndown\\s*=[0-9,\\s]+");
    if (rx.indexIn(description)!=-1) {
        QString valStr = rx.cap().split("=").at(1);
        QList<QString> strList = valStr.split(",");
        foreach (const QString & str, strList)
            burnDown.append(str.toInt());
    }

    rx.setPattern("capacity\\s*=[0-9\\s]+");
    if (rx.indexIn(description)!=-1) {
        QString str = rx.cap().split("=").at(1);
        capacity = str.toInt();
    }

    rx.setPattern("velocity\\s*=[0-9\\s]+");
    if (rx.indexIn(description)!=-1)
        velocity = rx.cap().split("=").at(1).toInt();

    rx.setPattern("workdays\\s*=[0-9\\s]+");
    if (rx.indexIn(description)!=-1)
        workDays = rx.cap().split("=").at(1).toInt();
}

void TracDataLoader::componentQueryResponseMethod(QVariant &arg)
{
    QStringList list = arg.toStringList();
    emit componentsLoaded(list);
}

void TracDataLoader::priorityQueryResponseMethod(QVariant &arg)
{
    QStringList list = arg.toStringList();
    emit prioritiesLoaded(list);
}

void TracDataLoader::typeQueryResponseMethod(QVariant &arg)
{
    QStringList list = arg.toStringList();
    emit typesLoaded(list);
}

void TracDataLoader::versionQueryResponseMethod(QVariant &arg)
{
    QStringList list = arg.toStringList();
    emit versionsLoaded(list);
}

void TracDataLoader::statusQueryResponseMethod(QVariant &arg)
{
    QStringList list = arg.toStringList();
    emit statusLoaded(list);
}

void TracDataLoader::estimationQueryResponseMethod(QVariant &arg)
{
    QStringList list = arg.toStringList();
    emit estimationsLoaded(list);
}



void TracDataLoader::updateStoriesResponseMethod( QVariant &arg)
{
    qDebug() << arg;
}


void TracDataLoader::saveNewStoryResponseMethod( QVariant &arg)
{
    QVariantList args;
    args << arg;
    rpc.call("ticket.get", args,
                this, SLOT(reloadNewStoryResponseMethod(QVariant &)),
                this, SLOT(myFaultResponse(int, const QString &)));

}

void TracDataLoader::reloadNewStoryResponseMethod(QVariant & arg)
{
    QVariantList fieldList = arg.toList();
    QVariantMap map = fieldList[3].toMap();

    QVariantMap newStory;
    newStory[storyFieldNames[ST_ID]]      = fieldList[0].toString();
    newStory[storyFieldNames[ST_DESC]]    = map["summary"].toString();
    newStory[storyFieldNames[ST_NOTES]]   = map["description"].toString();
    newStory[storyFieldNames[ST_HTD]]     = map["how_to_demo"].toString();
    newStory[storyFieldNames[ST_IMP]]     = map["priority"].toString();
    newStory[storyFieldNames[ST_EST]]     = map["estimation"].toString();
    newStory[storyFieldNames[ST_USER]]    = map["reporter"].toString();
    newStory[storyFieldNames[ST_TYP]]     = map["type"].toString();
    newStory[storyFieldNames[ST_STATUS]]  = map["milestone"].toString();
    newStory[storyFieldNames[ST_SPRINT]]  = map["component"].toString();
    newStory[storyFieldNames[ST_COMP]]    = map["version"].toString();
    newStory[storyFieldNames[ST_VERSION]] = map["status"].toString();

    emit newStoryLoaded(newStory);
}



void TracDataLoader::myFaultResponse(int error, const QString &message) {
    QString msg = QString().sprintf("An Error occured: %i. Message: ", error) + message;
    qDebug() << msg;
//    ui->statusBar->showMessage(msg, 5000);
}

