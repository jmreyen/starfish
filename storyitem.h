#ifndef STORYITEM_H
#define STORYITEM_H

#include "storydata.h"

class StoryItem
{
public:
    StoryItem(const QVariantList &list, StoryItem *parent = 0);
    StoryItem(const QVariantMap &map, StoryItem *parent = 0);
     ~StoryItem();

     void appendChild(StoryItem *child);

     StoryItem *child(int row);
     int childCount() const;
     int columnCount() const;
     QVariant data(int column) const;
     int row() const;
     StoryItem *parent();

     const StoryItem &fromMap(const QVariantMap &map);


 private:
     QList<StoryItem*> childItems;
     QList<QVariant> itemData;
     StoryItem *parentItem;
};

#endif // STORYITEM_H
