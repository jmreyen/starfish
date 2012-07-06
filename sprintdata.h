#ifndef SPRINTDATA_H
#define SPRINTDATA_H

#include <QString>
#include <QDate>
#include <QBool>
#include <QList>
#include <QVariant>



const int SP_NAME = 0;
const int SP_DUE  = 1;
const int SP_COMP = 2;
const int SP_DESC = 3;
const int SP_CAP  = 4;
const int SP_VEL  = 5;
const int SP_DAYS = 6;
const int SP_BURN = 7;
const int SP_LAST = 8;

extern const char *sprintFieldNames[SP_LAST];

class SprintData
{
public:
    SprintData(){}
    SprintData(const QVariantMap &map){fromMap(map);}
    SprintData(const QString &n, const QDate &d, bool b, const QString &dsc, int cap, int vel, int wd, const QList<int> &bd);

    QString name() const  {return theName;}
    QDate dueDate() const  {return theDueDate;}
    bool completed() const {return isCompleted;}
    int capacity()  const {return theCapacity;}
    int workDays() const {return theWorkDays;}
    const QList<int> & burnDown() const {return theBurnDown;}
    QVariant data(int n) const;
    bool setData(int section, QVariant value);
    const SprintData &fromMap(const QVariantMap &map);
    QVariantMap toMap() const;
    static QVariantMap toMap(const QString &name, const QDate &date, bool completed, const QString &desc, int capacity, int theVelocity, int workdays, const QList<int> &burndown);
protected:

private:
    QString theName;
    QDate   theDueDate;
    bool    isCompleted;
    QString theDescription;
    int     theCapacity;
    int     theVelocity;
    int     theWorkDays;
    QList<int> theBurnDown;
};

#endif // SPRINTDATA_H
