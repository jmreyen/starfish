
#include <QList>
#include <QVariant>

#include "storyitem.h"
#include "fields.h"




const char *storyDisplayNames[ST_LAST] = {
    "ID",
    "Description",
    "Notes",
    "How to Demo",
    "Importance",
    "Estimation",
    "User",
    "Type",
    "Status",
    "Sprint",
    "Component",
    "Version",
    "Parent",
    "Print"};

StoryItem::StoryItem(const QList<QVariant> &list, StoryItem *parent) :
    thePrintFlag(false),
    sortPosition(-1)

{
    itemData = list;
    setParent(parent);
}

StoryItem::StoryItem(const QVariantMap &map, StoryItem *parent) :
    thePrintFlag(false),
    sortPosition(-1)
{
    fromMap(map);
    setParent(parent);
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

bool StoryItem::setData(int column, const QVariant &value)
{
    itemData[column] = value;
    return true;
}


StoryItem *StoryItem::parent()
{
    return parentItem;
}

void StoryItem::setParent(StoryItem *parent)
{
    parentItem = parent;
    if (parent && parent->parent()) {
        QString parentID = parent->data(ST_ID).toString();
        setData(ST_PARENT, parentID);
    }
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

void StoryItem::clear()
{
    childItems.clear();
}

