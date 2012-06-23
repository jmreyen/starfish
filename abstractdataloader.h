#ifndef ABSTRACTDATALOADER_H
#define ABSTRACTDATALOADER_H

#include "storymodel.h"
#include "sprintmodel.h"

#include <QObject>

class AbstractDataLoader : public QObject
{
    Q_OBJECT
public:
    explicit AbstractDataLoader(StoryModel  &st, SprintModel &sp, QObject *parent = 0);
    virtual bool loadStories() = 0;
    virtual bool loadSprints() = 0;
protected:
    void addStory(int id, const QString &sum, const QString &desc, const QString &htd, const QString &prio, const QString &est, const QString &usr, const QString &ms, const QString &stat);
    void addSprint(const QString &name, const QDate &date, bool completed, const QString &desc);
signals:

public slots:

private:
    StoryModel  &theStories;
    SprintModel &theSprints;
};

#endif // ABSTRACTDATALOADER_H
