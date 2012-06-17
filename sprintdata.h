#ifndef SPRINTDATA_H
#define SPRINTDATA_H

#include <QString>
#include <QDate>
#include <QBool>
#include <QList>

class SprintData
{
public:
    SprintData(QString , QDate , bool, QString );
    QString name() const  {return theName;}
    QDate dueDate() const  {return theDueDate;}

protected:
    void parseDescription();

private:
    QString theName;
    QDate theDueDate;
    QBool completed;
    QString description;
    int capacity;
    int velocity;
    int workDays;
    QList<int> burnDown;
};

#endif // SPRINTDATA_H
