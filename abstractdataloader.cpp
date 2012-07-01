#include "abstractdataloader.h"

#include <QStandardItemModel>

AbstractDataLoader::AbstractDataLoader(
        StoryModel &sm,
        SprintModel &sp,
        QAbstractItemModel *pr,
        QAbstractItemModel *es,
        QAbstractItemModel *co,
        QAbstractItemModel *ve,
        QAbstractItemModel *ty,
        QAbstractItemModel *st,
        QObject *parent) :
    QObject(parent),
    theStories(sm),
    theSprints(sp),
    thePriorities(pr),
    theEstimations(es),
    theComponents(co),
    theVersions(ve),
    theTypes(ty),
    theStatus(st)
{
}

QList <StoryData> theStoryList;

void AbstractDataLoader::beginAddStory()
{
    theStoryList.clear();
}

void AbstractDataLoader::addStory(int id, const QString &sum, const QString &desc, const QString &htd, const QString &prio, const QString &est, const QString &usr, const QString &typ, const QString &ms, const QString &co, const QString &ve, const QString &stat)
{
    StoryData t(QString::number(id), sum, desc, htd, prio, est, usr, typ, ms, co, ve, stat);
    theStoryList.append(t);
}

void AbstractDataLoader::endAddStory()
{
    theStories.addStoryList(theStoryList);
}

void AbstractDataLoader::addSprint(const QString &name, const QDate &date, bool completed, const QString &description)
{
    SprintData s(name, date, completed, description);
    theSprints.addSprint(s);
}

void AbstractDataLoader::setPriorities(const QStringList &l)
{
    setStandardItemModel(l, thePriorities);
}

void AbstractDataLoader::setEstimations(const QStringList &l)
{
    setStandardItemModel(l, theEstimations);
}
void AbstractDataLoader::setVersions(const QStringList &l)
{
    setStandardItemModel(l, theVersions);
}
void AbstractDataLoader::setComponents(const QStringList &l)
{
    setStandardItemModel(l, theComponents);
}
void AbstractDataLoader::setTypes(const QStringList &l)
{
    setStandardItemModel(l, theTypes);
}
void AbstractDataLoader::setStatus(const QStringList &l)
{
    setStandardItemModel(l, theStatus);
}
void AbstractDataLoader::setStandardItemModel(const QStringList &l, QAbstractItemModel *m)
{
    QStandardItemModel *ms = static_cast<QStandardItemModel *>(m);
    foreach (const QString &str, l)
        ms->appendRow(new QStandardItem(str));
}

bool AbstractDataLoader::saveNewStories()
{
    for (int i = 0; i < theStories.rowCount(); ++i){
        StoryData d = theStories.story(i);
        if (d[ST_ID].toInt()==-1)
            saveNewStory(d);
    }
    return true;
}
