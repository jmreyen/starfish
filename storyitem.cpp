#include "storyitem.h"
#include "fields.h"

#include <QList>
#include <QVariant>
#include <QXmlStreamWriter>



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

StoryItem::StoryItem(const QString &desc, StoryItem *parent)
{
    for (int i=0; i< ST_LAST; ++i) {
        itemData.append("");
    }
    itemData[ST_DESC] = desc;
    setParent(parent);
}


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

StoryItem::StoryItem(QXmlStreamReader *reader, StoryItem *parent) :
    thePrintFlag(false),
    sortPosition(-1)
{
    readStory(reader);
    setParent(parent);
}

StoryItem::~StoryItem()
{
    qDeleteAll(childItems);
}

void StoryItem::addChild(StoryItem *item)
{
    childItems.append(item);
}

StoryItem *StoryItem::childAt(int row)
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

bool StoryItem::insertChild(int row, StoryItem *item)
{
    childItems.insert(row, item);
    return true;
}

StoryItem *StoryItem::takeChild(int row)
{
    StoryItem *item = childItems.takeAt(row);
    Q_ASSERT(item);
    item->setParent(0);
    return item;
}


StoryItem *StoryItem::parent() const
{
    return parentItem;
}

void StoryItem::setParent(StoryItem *parent)
{
    parentItem = parent;
    if (parent/* && parent->parent()*/) {
        QString parentID = parent->data(ST_ID).toString();
        setData(ST_PARENT, parentID);
        parent->addChild(this);
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
    for (int i=ST_FIRST; i<ST_LAST;++i) {
        itemData.append(map[storyFieldNames[i]]);
    }
    return *this;
}

void StoryItem::writeStory(QXmlStreamWriter *writer) const
{
    for (int i=ST_FIRST; i<ST_LAST;++i) {
        writer->writeAttribute(storyFieldNames[i], itemData.at(i).toString());
    }
}

void StoryItem::readStory(QXmlStreamReader *reader)
{
    for (int i=ST_FIRST; i<ST_LAST;++i) {
        itemData.append(reader->attributes().value(storyFieldNames[i]).toString());
    }
}

void StoryItem::clear()
{
    childItems.clear();
}

