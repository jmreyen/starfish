#ifndef STORYDATA_H
#define STORYDATA_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QVariant>

const int ST_ID     = 0;
const int ST_DESC   = 1;
const int ST_NOTES  = 2;
const int ST_HTD    = 3;
const int ST_IMP    = 4;
const int ST_EST    = 5;
const int ST_USER   = 6;
const int ST_TYP    = 7;
const int ST_STATUS = 8;
const int ST_SPRINT = 9;
const int ST_COMP   = 10;
const int ST_VERSION= 11;
const int ST_LAST   = 12;

extern const char *storyFieldNames[];
extern const char *storyDisplayNames[];

class StoryData : public QVector<QString>
{
public:
    StoryData() : QVector<QString>(ST_LAST){}
    StoryData(const QString &id,
              const QString &desc,
              const QString &notes,
              const QString &htd,
              const QString &imp,
              const QString &est,
              const QString &user,
              const QString &typ,
              const QString &sprint,
              const QString &comp,
              const QString &ver,
              const QString &status) : QVector<QString>(ST_LAST)
    {


        (*this)[ST_ID] = id;
        (*this)[ST_DESC] = desc;
        (*this)[ST_NOTES] = notes;
        (*this)[ST_HTD] = htd;
        (*this)[ST_IMP] = imp;
        (*this)[ST_EST] = est;
        (*this)[ST_USER] = user;
        (*this)[ST_TYP] = typ;
        (*this)[ST_SPRINT] = sprint;
        (*this)[ST_COMP] = comp;
        (*this)[ST_VERSION] = ver;
        (*this)[ST_STATUS] = status;
    }
    StoryData(const QVariantMap &map){fromMap(map);}
    StoryData(const StoryData &t) : QVector<QString>(ST_LAST)
    {
        (*this) = t;
    }
    QVariantMap toMap() const;
    static QVariantMap toMap(const QString &id,
                             const QString &desc,
                             const QString &notes,
                             const QString &htd,
                             const QString &imp,
                             const QString &est,
                             const QString &user,
                             const QString &typ,
                             const QString &sprint,
                             const QString &comp,
                             const QString &ver,
                             const QString status);
    const StoryData &fromMap(const QVariantMap &map);

};



#endif // STORYDATA_H
