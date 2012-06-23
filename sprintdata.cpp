#include "sprintdata.h"

#include <QVariant>
#include <QRegExp>
#include <QStringList>
#include <QDebug>

SprintData::SprintData(QString n, QDate d, bool b, QString dsc):
    theName(n),
    theDueDate(d),
    isCompleted(b),
    description(dsc),
    theCapacity(0),
    velocity(0),
    theWorkDays(0)
{
    parseDescription();
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
    case SP_VEL:  return velocity;
    case SP_DAYS: return theWorkDays;
    case SP_BURN: return toVariantList(theBurnDown);
    }
    return QVariant();
}

bool SprintData::setData(int section, QVariant value)
{
    switch (section) {
    case SP_NAME:   theName = value.toString(); break;
    case SP_DUE:    theDueDate = value.toDate();  break;
    case SP_CAP:    theCapacity = value.toInt(); break;
    case SP_VEL:    velocity = value.toInt();  break;
    case SP_DAYS:   theWorkDays = value.toInt();  break;
    case SP_BURN:   theBurnDown.clear();
                    foreach (QVariant v, value.toList())
                        theBurnDown.append(v.toInt());
                    break;
    default: return false;
    }
    return true;
}



void SprintData::parseDescription()
{
    QRegExp rx("burndown\\s*=[0-9,\\s]+");
    if (rx.indexIn(description)!=-1) {
        QString valStr = rx.cap().split("=").at(1);
        QList<QString> strList = valStr.split(",");
        foreach (const QString & str, strList)
            theBurnDown.append(str.toInt());
    }

    rx.setPattern("capacity\\s*=[0-9\\s]+");
    if (rx.indexIn(description)!=-1) {
        QString str = rx.cap().split("=").at(1);
        theCapacity = str.toInt();
    }

    rx.setPattern("velocity\\s*=[0-9\\s]+");
    if (rx.indexIn(description)!=-1)
        velocity = rx.cap().split("=").at(1).toInt();

    rx.setPattern("workdays\\s*=[0-9\\s]+");
    if (rx.indexIn(description)!=-1)
        theWorkDays = rx.cap().split("=").at(1).toInt();

}
