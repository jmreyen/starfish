#ifndef ABSTRACTDATALOADER_H
#define ABSTRACTDATALOADER_H

#include <QObject>
#include <QVariant>

class AbstractDataLoader : public QObject
{
    Q_OBJECT
public:
    explicit AbstractDataLoader(QObject *parent = 0) {}
    virtual bool load() = 0;
    virtual bool saveNewStory(const QVariantMap &map) = 0;
    virtual bool updateStories(QMap<QString, QVariantMap> &map) = 0;

signals:
    void storiesLoaded(const QVariantList &l);
    void newStoryLoaded(const QVariantMap &m);
    void sprintsLoaded(const QVariantList &list);
    void prioritiesLoaded(const QStringList &list);
    void estimationsLoaded(const QStringList &list);
    void versionsLoaded(const QStringList &list);
    void componentsLoaded(const QStringList &list);
    void statusLoaded(const QStringList &list);
    void typesLoaded(const QStringList &list);

public slots:
    void onSaveNewStory(const QVariantMap &map){saveNewStory(map);}

protected:

private:

};

#endif // ABSTRACTDATALOADER_H
