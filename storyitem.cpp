
#include <QList>
#include <QVariant>


#include "storyitem.h"



StoryItem::StoryItem(const QList<QVariant> &list, StoryItem *parent)
{
    parentItem = parent;
    itemData = list;
}

StoryItem::StoryItem(const QVariantMap &map, StoryItem *parent)
{
    parentItem = parent;
    fromMap(map);
}

StoryItem::~StoryItem()
{
    qDeleteAll(childItems);
}

void StoryItem::appendChild(StoryItem *item)
{
    childItems.append(item);
}

StoryItem *StoryItem::child(int row)
{
    return childItems.value(row);
}

int StoryItem::childCount() const
{
    return childItems.count();
}

int StoryItem::columnCount() const
{
    return itemData.count();
}

QVariant StoryItem::data(int column) const
{
    return itemData.value(column);
}

StoryItem *StoryItem::parent()
{
    return parentItem;
}

int StoryItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<StoryItem*>(this));

    return 0;
}

const StoryItem &StoryItem::fromMap(const QVariantMap &map)
{
    for (int i=ST_ID; i<ST_LAST;++i) {
        itemData.append(map[storyFieldNames[i]]);
    }
    return *this;
}
