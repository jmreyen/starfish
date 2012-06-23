#ifndef STORYDATA_H
#define STORYDATA_H

#include <QString>
#include <QVector>

const int ST_ID     = 0;
const int ST_DESC   = 1;
const int ST_NOTES  = 2;
const int ST_HTD    = 3;
const int ST_IMP    = 4;
const int ST_EST    = 5;
const int ST_USER   = 6;
const int ST_SPRINT = 7;
const int ST_STATUS = 8;
const int ST_LAST   = 9;


class StoryData : public QVector<QString>
{
public:
    StoryData() : QVector<QString>(ST_LAST){}
    StoryData(QString id,
               QString desc,
               QString notes,
               QString htd,
               QString imp,
               QString est,
               QString user,
               QString sprint,
               QString status) : QVector<QString>(ST_LAST)
    {


        (*this)[ST_ID] = id;
        (*this)[ST_DESC] = desc;
        (*this)[ST_NOTES] = notes;
        (*this)[ST_HTD] = htd;
        (*this)[ST_IMP] = imp;
        (*this)[ST_EST] = est;
        (*this)[ST_USER] = user;
        (*this)[ST_SPRINT] = sprint;
        (*this)[ST_STATUS] = status;
    }
    StoryData(const StoryData &t) : QVector<QString>(ST_LAST)
     {
        (*this) = t;
     }

};



#endif // STORYDATA_H
