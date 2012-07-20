#ifndef SPRINTDATA_H
#define SPRINTDATA_H

#include <QString>
#include <QDate>
#include <QBool>
#include <QList>
#include <QVariant>




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
