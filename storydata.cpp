#include "storydata.h"

const char *storyFieldNames[] = {
    "id",
    "summary",
    "description",
    "how_to_demo",
    "priority",
    "estimation",
    "reporter",
    "type",
    "status",
    "milestone",
    "component",
    "version"};

const char *storyDisplayNames[ST_LAST] = {
    "ID",
    "Description",
    "Notes",
    "How to Demo",
    "Importance",
    "Estimation",
    "User",
    "Type",
    "Status",
    "Sprint",
    "Component",
    "Version"};

QVariantMap StoryData::toMap() const
{
    QVariantMap newMap;
    for (int i = 0; i < ST_LAST; ++i) {
        newMap[storyFieldNames[i]] = (*this)[i];
    }
    return newMap;
}

QVariantMap StoryData::toMap(const QString &id,
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
                             const QString status)
{
    QMap<QString, QVariant> newMap;
    newMap[storyFieldNames[ST_ID]]      = id;
    newMap[storyFieldNames[ST_DESC]]    = desc;
    newMap[storyFieldNames[ST_NOTES]]   = notes;
    newMap[storyFieldNames[ST_HTD]]     = htd;
    newMap[storyFieldNames[ST_IMP]]     = imp;
    newMap[storyFieldNames[ST_EST]]     = est;
    newMap[storyFieldNames[ST_USER]]    = user;
    newMap[storyFieldNames[ST_TYP]]     = typ;
    newMap[storyFieldNames[ST_STATUS]]  = status;
    newMap[storyFieldNames[ST_SPRINT]]  = sprint;
    newMap[storyFieldNames[ST_COMP]]    = comp;
    newMap[storyFieldNames[ST_VERSION]] = ver;
    return newMap;
}

const StoryData &StoryData::fromMap(const QVariantMap &map)
{
    for (int i=ST_ID; i<ST_LAST;++i) {
        (*this)[i] = map[storyFieldNames[i]].toString();
    }
    return *this;
}


