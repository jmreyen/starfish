#ifndef ABSTRACTDATALOADER_H
#define ABSTRACTDATALOADER_H

#include "storymodel.h"
#include "sprintmodel.h"

#include <QObject>
#include <QAbstractItemModel>

class AbstractDataLoader : public QObject
{
    Q_OBJECT
public:
    explicit AbstractDataLoader(StoryModel  &st, SprintModel &sp,  QAbstractItemModel *pr,
                                QAbstractItemModel *es, QAbstractItemModel *co,
                                QAbstractItemModel *ve, QAbstractItemModel *ty, QObject *parent = 0);
    virtual bool loadStories() = 0;
    virtual bool loadSprints() = 0;
    virtual bool loadMasterData() = 0;

signals:

public slots:

protected:
    void addStory(int id, const QString &sum, const QString &desc, const QString &htd, const QString &prio, const QString &est, const QString &usr, const QString &typ, const QString &ms, const QString &stat);
    void addSprint(const QString &name, const QDate &date, bool completed, const QString &desc);
    void setPriorities(const QStringList &l);
    void setEstimations(const QStringList &l);
    void setVersions(const QStringList &l);
    void setComponents(const QStringList &l);
    void setTypes(const QStringList &l);
private:
    void setStandardItemModel(const QStringList &l, QAbstractItemModel *m);

private:
    StoryModel  &theStories;
    SprintModel &theSprints;
    QAbstractItemModel *thePriorities;
    QAbstractItemModel *theEstimations;
    QAbstractItemModel *theComponents;
    QAbstractItemModel *theOwners;
    QAbstractItemModel *theVersions;
    QAbstractItemModel *theTypes;

};

#endif // ABSTRACTDATALOADER_H
