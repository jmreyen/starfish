#include "sprintdata.h"

#include <QVariant>
#include <QRegExp>
#include <QStringList>
#include <QDebug>

const char *sprintFieldNames[SP_LAST] = {
    "name",
    "due",
    "completed",
    "description",
    "capacity",
    "velocity",
    "workdays",
    "burndown"};

SprintData::SprintData(const QString &n, const QDate &d, bool b, const QString &dsc, int cap, int vel, int wd, const QList<int> &bd):
    theName(n),
    theDueDate(d),
    isCompleted(b),
    theDescription(dsc),
    theCapacity(cap),
    theVelocity(vel),
    theWorkDays(wd),
    theBurnDown(bd)
{
}

QVariantList toVariantList(const QList<int> &list)
{
    QVariantList tmp;
    for (QList<int>::const_iterator i = list.begin(); i!=list.end(); ++i)
        tmp.append(QVariant(*i));
    return tmp;
}


QVariant SprintData::data(int n) const
{
    switch (n) {
    case SP_NAME: return theName;
    case SP_DUE:  return theDueDate;
    case SP_CAP:  return theCapacity;
    case SP_VEL:  return theVelocity;
    case SP_DAYS: return theWorkDays;
    case SP_BURN: return toVariantList(theBurnDown);
    }
    return QVariant();
}

bool SprintData::setData(int section, QVariant value)
{
    switch (section) {
    case SP_NAME:   theName = value.toString(); break;
    case SP_DUE:    theDueDate = value.toDate(); break;
    case SP_CAP:    theCapacity = value.toInt(); break;
    case SP_VEL:    theVelocity = value.toInt();  break;
    case SP_DAYS:   theWorkDays = value.toInt(); break;
    case SP_BURN:   theBurnDown.clear();
                    foreach (QVariant v, value.toList())
                        theBurnDown.append(v.toInt());
                    break;
    default: return false;
    }
    return true;
}




QVariantMap SprintData::toMap() const
{
    return SprintData::toMap(
                theName,
                theDueDate,
                isCompleted,
                theDescription,
                theCapacity,
                theVelocity,
                theWorkDays,
                theBurnDown);
}

QVariantMap SprintData::toMap(
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

const SprintData &SprintData::fromMap(const QVariantMap &map)
{
    theName =        map[sprintFieldNames[SP_NAME]].toString();
    theDueDate =     map[sprintFieldNames[SP_DUE]].toDateTime().date();
    isCompleted =    map[sprintFieldNames[SP_COMP]].toBool();
    theDescription = map[sprintFieldNames[SP_DESC]].toString();
    theCapacity =    map[sprintFieldNames[SP_CAP]].toInt();
    theVelocity =    map[sprintFieldNames[SP_VEL]].toInt();
    theWorkDays =    map[sprintFieldNames[SP_DAYS]].toInt();
    foreach(QVariant v, map[sprintFieldNames[SP_BURN]].toList())
            theBurnDown.append(v.toInt());
    return *this;
}
