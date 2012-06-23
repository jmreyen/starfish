#include "abstractdataloader.h"

AbstractDataLoader::AbstractDataLoader(StoryModel &st, SprintModel &sp, QObject *parent) :
    QObject(parent),
    theStories(st),
    theSprints(sp)
{
}

void AbstractDataLoader::addStory(int id, const QString &sum, const QString &desc, const QString &htd, const QString &prio, const QString &est, const QString &usr, const QString &ms, const QString &stat)
{

    StoryData t(QString::number(id), sum, desc, htd, prio, est, usr, ms, stat);
    theStories.addTicket(t);
}

void AbstractDataLoader::addSprint(const QString &name, const QDate &date, bool completed, const QString &description)
{
    SprintData s(name, date, completed, description);
    theSprints.addSprint(s);
}
