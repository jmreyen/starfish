#ifndef STORYITEM_H
#define STORYITEM_H

#include <QVariant>

class QXmlStreamWriter;
class QXmlStreamReader;

extern const char *storyDisplayNames[];

class StoryItem
{
public:
    StoryItem(const QString &desc, StoryItem *parent = 0);
    StoryItem(const QVariantList &list, StoryItem *parent = 0);
    StoryItem(const QVariantMap &map, StoryItem *parent = 0);
    StoryItem(QXmlStreamReader *reader, StoryItem *parent);

    ~StoryItem();

    StoryItem *childAt(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool setData(int column, const QVariant &value);
    bool insertChild(int row, StoryItem *item);
    void addChild(StoryItem *childAt);
    StoryItem* takeChild(int row);
    int row() const;
    StoryItem *parent() const;
    void setParent(StoryItem *parent);

    void clear();
    void writeStory(QXmlStreamWriter *writer) const;

protected:
    const StoryItem &fromMap(const QVariantMap &map);
    void readStory(QXmlStreamReader *reader);

private:
    QList<StoryItem*> childItems;
    QList<QVariant> itemData;
    StoryItem *parentItem;
    int sortPosition;
};

#endif // STORYITEM_H
