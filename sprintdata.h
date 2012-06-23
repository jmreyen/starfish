#ifndef SPRINTDATA_H
#define SPRINTDATA_H

#include <QString>
#include <QDate>
#include <QBool>
#include <QList>
#include <QVariant>

const int SP_NAME = 0;
const int SP_DUE  = 1;
const int SP_CAP  = 2;
const int SP_VEL  = 3;
const int SP_DAYS = 4;
const int SP_BURN = 5;
const int SP_LAST = 6;

class SprintData
{
public:
    SprintData(){}
    SprintData(QString , QDate , bool, QString );

    QString name() const  {return theName;}
    QDate dueDate() const  {return theDueDate;}
    bool completed() const {return isCompleted;}
    int capacity()  const {return theCapacity;}
    int workDays() const {return theWorkDays;}
    const QList<int> & burnDown() const {return theBurnDown;}
    QVariant data(int n) const;
    bool setData(int section, QVariant value);

protected:
    void parseDescription();

private:
    QString theName;
    QDate   theDueDate;
    bool    isCompleted;
    QString description;
    int     theCapacity;
    int     velocity;
    int     theWorkDays;
    QList<int> theBurnDown;
};

#endif // SPRINTDATA_H
