#include "tracio.h"
#include "iofactory.h"
#include "fields.h"

namespace
{
    AbstractIO *CreateTracIO()
    {
        return new TracIO;
    }
    const QString Trac = "TRAC";
    const bool registered = IOFactory::Instance()->RegisterIO(Trac, CreateTracIO);
}

QStringList picStatusList = QString("new,postponed,prioritised,selected,waiting,done").split(",");
QStringList tracStatusList = QString("new,postponed,accepted,assigned,waiting,closed").split(",");

void toTrac(QVariantMap &map)
{
    QString picStatus = map[storyFieldNames[ST_STATUS]].toString();
    int n = picStatusList.indexOf(picStatus);
    if (n != -1) {
        QString tracStatus = tracStatusList[n];
        map[storyFieldNames[ST_STATUS]] = tracStatus;
    }
}

void toPic(QVariantMap &map)
{
    QString tracStatus = map[storyFieldNames[ST_STATUS]].toString();
    int n = tracStatusList.indexOf(tracStatus);
    if (n != -1) {
        QString picStatus = picStatusList[n];
        map[storyFieldNames[ST_STATUS]] = picStatus;
    }
}

TracIO::TracIO(QObject *parent) :
    AbstractIO(parent),
    rpc(this),
    theUrl()
{
}


bool TracIO::saveNewStory(const QVariantMap &map)
{
    QVariantList args;
    QVariantMap attributes;
    args.append(map[storyFieldNames[ST_DESC]]);
    args.append(map[storyFieldNames[ST_NOTES]]);
    for (int i=ST_HTD; i < ST_IO_LAST; ++i) {
        attributes.insert(storyFieldNames[i], map[storyFieldNames[i]]);
    }
    toTrac(attributes);
    args.append(attributes);
    rpc.call("ticket.create", args,
                this, SLOT(saveNewStoryResponseMethod(QVariant &)),
                this, SLOT(myFaultResponse(int, const QString &)));
    return true;
}



bool TracIO::updateStories(QMap<QString, QVariantMap> & map)
{
    QVariantList args, methodList;

    for (QMap<QString, QVariantMap>::iterator itr = map.begin(); itr != map.end(); ++itr) {

        QVariantMap newMethod;
        QVariantList newParams;
        QVariantMap newAttributes = itr.value();
        toTrac(newAttributes);
        newAttributes.insert("action", "leave");
        newParams.append(itr.key().toInt());
        newParams.append("Stored by PIC");
        newParams.append(newAttributes);
        newMethod.insert("methodName", "ticket.update");
        newMethod.insert("params", newParams);
        methodList.append(newMethod);
    }

    args.insert(0, methodList);

    qDebug() << "*** RPC Input Map ***\n" << args <<"\n";

    rpc.call("system.multicall", args,
             this, SLOT(updateStoriesResponseMethod(QVariant&)),
             this, SLOT(myFaultResponse(int, const QString &)));
    return true;
}



bool TracIO::load()
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

void TracIO::ticketQueryResponseMethod(QVariant &arg) {
    QStringList list = arg.toStringList();
    QVariantList args, methodList;

    emit message("Retrieving ...");
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

void TracIO::getTicketResponseMethod(QVariant &arg)
{
    QVariantList ticketList = arg.toList();
    QVariantList storyList;
    for (int i = 0; i < ticketList.size(); ++i) {
        QVariantList fieldList = ticketList[i].toList().at(0).toList();
        QMap<QString,QVariant> fieldMap = fieldList[3].toMap();
        QVariantMap newStory;
        newStory[storyFieldNames[ST_ID]]      = fieldList[0].toString();
        newStory[storyFieldNames[ST_DESC]]    = fieldMap["summary"].toString();
        newStory[storyFieldNames[ST_NOTES]]   = fieldMap["description"].toString();
        newStory[storyFieldNames[ST_HTD]]     = fieldMap["how_to_demo"].toString();
        newStory[storyFieldNames[ST_IMP]]     = fieldMap["priority"].toString();
        newStory[storyFieldNames[ST_EST]]     = fieldMap["estimation"].toString();
        newStory[storyFieldNames[ST_USER]]    = fieldMap["reporter"].toString();
        newStory[storyFieldNames[ST_TYP]]     = fieldMap["type"].toString();
        newStory[storyFieldNames[ST_STATUS]]  = fieldMap["status"].toString();
        newStory[storyFieldNames[ST_SPRINT]]  = fieldMap["milestone"].toString();
        newStory[storyFieldNames[ST_COMP]]    = fieldMap["component"].toString();
        newStory[storyFieldNames[ST_VERSION]] = fieldMap["version"].toString();
        newStory[storyFieldNames[ST_PARENT]]  = fieldMap["parent"].toString();
        toPic(newStory);
        storyList.append(newStory);
    }
    emit storiesLoaded(storyList);
    emit message("");
}

void TracIO::sprintQueryResponseMethod(QVariant &arg) {

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

QVariantMap toSprintMap(
        const QString &name,
        const QDate &date,
        bool completed,
        const QString &desc,
        int capacity,
        int velocity,
        int workdays,
        const QList<int> &burndown)
{
    QVariantMap newMap;
    QVariantList newBurnDownList;
    foreach (int n, burndown)
        newBurnDownList.append(n);
    newMap[sprintFieldNames[SP_NAME]] = name;
    newMap[sprintFieldNames[SP_DUE]]  = date;
    newMap[sprintFieldNames[SP_COMP]] = completed;
    newMap[sprintFieldNames[SP_DESC]] = desc;
    newMap[sprintFieldNames[SP_CAP]]  = capacity;
    newMap[sprintFieldNames[SP_VEL]]  = velocity;
    newMap[sprintFieldNames[SP_DAYS]] = workdays;
    newMap[sprintFieldNames[SP_BURN]] = newBurnDownList;

    return newMap;
}


void TracIO::getSprintResponseMethod(QVariant &arg)
{
    QVariantList sprintList = arg.toList();
    QVariantList newList;
    for (int i = 0; i < sprintList.size(); ++i) {
        QMap<QString, QVariant> map = sprintList[i].toList().at(0).toMap();
        int capacity = 0, velocity = 0, workdays = 0;
        QList<int> burnDown;
        parseSprintDescription(map["description"].toString(), capacity, velocity, workdays, burnDown);
        QVariantMap newMap = toSprintMap(
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

void TracIO::parseSprintDescription(const QString &description, int &capacity, int &velocity, int &workDays, QList<int> &burnDown) const
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

void TracIO::componentQueryResponseMethod(QVariant &arg)
{
    QStringList list = arg.toStringList();
    emit componentsLoaded(list);
}

void TracIO::priorityQueryResponseMethod(QVariant &arg)
{
    QStringList list = arg.toStringList();
    emit prioritiesLoaded(list);
}

void TracIO::typeQueryResponseMethod(QVariant &arg)
{
    QStringList list = arg.toStringList();
    emit typesLoaded(list);
}

void TracIO::versionQueryResponseMethod(QVariant &arg)
{
    QStringList list = arg.toStringList();
    emit versionsLoaded(list);
}

void TracIO::statusQueryResponseMethod(QVariant &arg)
{
    QStringList list = arg.toStringList();
    foreach(const QString &s, tracStatusList) {
        if (!list.contains(s))
            emit loadError("Trac workflow not set up properly. Status " +s+" is missing.");
    }

    foreach(const QString &s, list) {
        if (!tracStatusList.contains(s))
            emit loadError("Additional status " +s+" found in Trac setup. It will be ignored.");
    }

    emit statusLoaded(picStatusList);
}

void TracIO::estimationQueryResponseMethod(QVariant &arg)
{
    QStringList list = arg.toStringList();
    emit estimationsLoaded(list);
}



void TracIO::updateStoriesResponseMethod( QVariant &arg)
{
    qDebug() << "*** Story Updated *** \n" << arg << "\n";
}


void TracIO::saveNewStoryResponseMethod( QVariant &arg)
{
    QVariantList args;
    args << arg;
    rpc.call("ticket.get", args,
                this, SLOT(reloadNewStoryResponseMethod(QVariant &)),
                this, SLOT(myFaultResponse(int, const QString &)));

}

void TracIO::reloadNewStoryResponseMethod(QVariant & arg)
{
    QVariantList fieldList = arg.toList();
    QVariantMap fieldMap = fieldList[3].toMap();

    QVariantMap newStory;
    newStory[storyFieldNames[ST_ID]]      = fieldList[0].toString();
    newStory[storyFieldNames[ST_DESC]]    = fieldMap["summary"].toString();
    newStory[storyFieldNames[ST_NOTES]]   = fieldMap["description"].toString();
    newStory[storyFieldNames[ST_HTD]]     = fieldMap["how_to_demo"].toString();
    newStory[storyFieldNames[ST_IMP]]     = fieldMap["priority"].toString();
    newStory[storyFieldNames[ST_EST]]     = fieldMap["estimation"].toString();
    newStory[storyFieldNames[ST_USER]]    = fieldMap["reporter"].toString();
    newStory[storyFieldNames[ST_TYP]]     = fieldMap["type"].toString();
    newStory[storyFieldNames[ST_STATUS]]  = fieldMap["status"].toString();
    newStory[storyFieldNames[ST_SPRINT]]  = fieldMap["milestone"].toString();
    newStory[storyFieldNames[ST_COMP]]    = fieldMap["component"].toString();
    newStory[storyFieldNames[ST_VERSION]] = fieldMap["version"].toString();
    newStory[storyFieldNames[ST_PARENT]]  = fieldMap["parent"].toString();;
    toPic(newStory);
    emit newStoryLoaded(newStory);
}



void TracIO::myFaultResponse(int error, const QString &message) {
    QString msg = QString().sprintf("An Error occured: %i. Message: ", error) + message;
    qDebug() << msg;
    //emit message(msg);
}

bool TracIO::loadSettings(const QSettings &settings)
{
    QUrl url;
    url.setUrl(settings.value("TRAC/Server").toString());
    url.setPort(settings.value("TRAC/Port").toInt());
    url.setUserName(settings.value("TRAC/UserName").toString());
    url.setPassword(settings.value("TRAC/Password").toString());
    setUrl(url);
    setQueryString(settings.value("TRAC/QueryString").toString());
    return true;
}

bool TracIO::saveSettings(QSettings &settings) const
{
    settings.setValue("TRAC/Server",url().toString(QUrl::RemoveUserInfo|QUrl::RemovePort));
    settings.setValue("TRAC/Port", url().port());
    settings.setValue("TRAC/UserName", url().userName());
    settings.setValue("TRAC/Password", url().password());
    settings.setValue("TRAC/QueryString", queryString());
    return true;
}

QFrame *TracIO::getSettingsFrame()
{
    QFrame *theWidget;

    theWidget = new QFrame();
    ui.setupUi(theWidget);

    ui.editTracServer->setText(url().toString(QUrl::RemoveUserInfo|QUrl::RemovePort));
    ui.spinBoxTracPort->setValue(url().port());
    ui.editTracUser->setText(url().userName());
    ui.editTracPassword->setText(url().password());
    ui.editTracQueryFilter->setText(theQueryString);

    return theWidget;
}

void TracIO::acceptSetup()
{
    QUrl url;
    url.setUrl(ui.editTracServer->text());
    url.setPort(ui.spinBoxTracPort->value());
    url.setUserName(ui.editTracUser->text());
    url.setPassword(ui.editTracPassword->text());
    setUrl(url);
    setQueryString(ui.editTracQueryFilter->text());
}


