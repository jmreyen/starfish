#ifndef TICKETDATA_H
#define TICKETDATA_H

#include <QString>
#include <QVector>

const int TD_ID     = 0;
const int TD_DESC   = 1;
const int TD_NOTES  = 2;
const int TD_HTD    = 3;
const int TD_IMP    = 4;
const int TD_EST    = 5;
const int TD_USER   = 6;
const int TD_SPRINT = 7;
const int TD_STATUS = 8;
const int TD_LAST   = 9;


class TicketData : public QVector<QString>
{
public:
    TicketData() : QVector<QString>(10){}
    TicketData(QString id,
               QString desc,
               QString notes,
               QString htd,
               QString imp,
               QString est,
               QString user,
               QString sprint,
               QString status) : QVector<QString>(10)
    {


        (*this)[TD_ID] = id;
        (*this)[TD_DESC] = desc;
        (*this)[TD_NOTES] = notes;
        (*this)[TD_HTD] = htd;
        (*this)[TD_IMP] = imp;
        (*this)[TD_EST] = est;
        (*this)[TD_USER] = user;
        (*this)[TD_SPRINT] = sprint;
        (*this)[TD_STATUS] = status;
    }
    TicketData(const TicketData &t) : QVector<QString>(10)
     {
        (*this) = t;
     }

};



#endif // TICKETDATA_H
