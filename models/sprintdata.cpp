#include "models/sprintdata.h"
#include "base/fields.h"

#include <QVariant>
#include <QRegExp>
#include <QStringList>
#include <QDebug>


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
