#ifndef ABSTRACTDATALOADER_H
#define ABSTRACTDATALOADER_H

#include <QObject>
#include <QVariant>
#include <QSettings>
#include <QFrame>

class AbstractIO : public QObject
{
    Q_OBJECT
public:
    explicit AbstractIO(QObject *parent = 0) {}
    virtual bool load() = 0;
    virtual bool saveNewStory(const QVariantMap &map) = 0;
    virtual bool updateStories(QMap<QString, QVariantMap> &map) = 0;
    virtual bool loadSettings(const QSettings &settings) = 0;
    virtual bool saveSettings(QSettings &settings) const = 0;
    virtual QFrame *getSettingsFrame()  = 0;

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
    void onSetupAccepted(){acceptSetup();}

protected:
    virtual void acceptSetup() = 0;

private:

};

#endif // ABSTRACTDATALOADER_H
