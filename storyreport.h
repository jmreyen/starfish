#ifndef STORYREPORT_H
#define STORYREPORT_H

#include <QString>
#include <QMap>

class StoryItem;

class AbstractStoryReport  : public QString
{
public:
    explicit AbstractStoryReport();
    virtual void beginInsertStory(const QString &header) = 0;
    virtual void insertStory(const StoryItem &story) = 0;
    virtual void endInsertStory() = 0;

protected:
    QString &wiki2html(QString & str);

};


class StoryReport : public AbstractStoryReport
{
public:
    explicit StoryReport();
    void beginInsertStory(const QString &header);
    void insertStory(const StoryItem &story);
    void endInsertStory();
protected:
    QMap <QString, QString> theMap;
};

class QuickReport : public AbstractStoryReport
{
public:
    explicit QuickReport();
    void beginInsertStory(const QString &header);
    void insertStory(const StoryItem &story);
    void endInsertStory();
protected:
    QMap <QString, QString> theMap;
};

class MeetingHandout : public AbstractStoryReport
{
public:
    explicit MeetingHandout();
    void beginInsertStory(const QString &header);
    void insertStory(const StoryItem &story);
    void endInsertStory();
protected:
    QMap <QString, QString > theMap;
    QString theSprintReview;
    QString theNewStories;
};


#endif // STORYREPORT_H
