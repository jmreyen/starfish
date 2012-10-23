#include "storyitem.h"
#include "storyitemmodel.h"

#include <QtGui>
#include <QXmlStreamWriter>

// class StoryIterator
StoryIterator::StoryIterator(StoryItem *item):
    theRootItem(item)

{
    makeFlatList(theRootItem);
    if (theFlatList.count() == 0)
        current = -1;
    else
        current = 0;
}

void StoryIterator::makeFlatList(StoryItem *item)
{
    for (int i = 0; i<item->childCount(); ++i) {
        theFlatList.append(QPair<StoryItem *, int>(item->childAt(i), i));
        makeFlatList(item->childAt(i));
    }
}

StoryIterator &StoryIterator::operator = (StoryIterator &iterator)
{
    theFlatList = iterator.theFlatList;
    current=iterator.current;
    return *this;
}


StoryIterator &StoryIterator::operator ++()
{
    ++current;
    if (current == theFlatList.count())
        current = -1;
    return *this;
}

const StoryItem *StoryIterator::operator ->()
{
    return theFlatList[current].first;
}

const StoryItem *StoryIterator::operator *()
{
    return theFlatList[current].first;
}


bool StoryIterator::operator !=(const StoryIterator &iterator) const
{
    return current!=iterator.current;
}



// class StoryItemModel

StoryItemModel::StoryItemModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    for (int i=ST_ID; i<ST_LAST; ++i)
        rootData << storyDisplayNames[i];
    rootItem = new StoryItem(rootData);
}

StoryItemModel::~StoryItemModel()
{
    delete rootItem;
}

// >>> model/view methods (from QAbstractItemModel

QVariant StoryItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    StoryItem *item = getItem(index);

    switch (role) {
    case Qt::EditRole:
    case Qt::DisplayRole:
        if (index.column() < ST_LAST)
            return item->data(index.column());
        break;
    case Qt::CheckStateRole:
        //Get status of the checkbox in the print column
        if (index.column()== ST_PRINT)
            return item->data(index.column()).toBool();
        break;
    case Qt::FontRole: {
            if (item->data(ST_MODIFIED).toBool()) {
                QFont font;
                font.setItalic(true);
                return font;
            }
        }
        break;
    }
    return QVariant();
}

Qt::ItemFlags StoryItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled;

    Qt::ItemFlags f =
            Qt::ItemIsEnabled |
            Qt::ItemIsSelectable;

   if (index.column() == ST_ID)
            f = f | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

    if (index.column()==ST_PRINT)
        f = f | Qt::ItemIsUserCheckable;

    return f;
}

QVariant StoryItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        if (orientation == Qt::Horizontal/* && section != ST_PRINT*/)
            return rootItem->data(section);
        break;
    case Qt::DecorationRole:
        if (orientation == Qt::Horizontal && section == ST_PRINT)
            return QIcon(":images/printer.png");
        break;
    }
    return QVariant();
}

int StoryItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    return getItem(parent)->childCount();
}

int StoryItemModel::columnCount(const QModelIndex &parent) const
{
    return getItem(parent)->columnCount();
}

bool StoryItemModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
    if (!index.isValid())
        return false;

    StoryItem *item = getItem(index);

    switch (role){
    case Qt::DisplayRole: //Hmm, warum habe ich das denn gebraucht?!?
    case Qt::EditRole:
        if (index.column() < ST_LAST) {
            if (item->data(index.column()) != value) {
                item->setData(index.column(), value);
                emit dataChanged(index, index);
                return true;
            }
        }
        break;
    case Qt::CheckStateRole :
        if (index.column()==ST_PRINT) {
            //Set status of the checkbox in the print column
            item->setData(index.column(), !item->data(index.column()).toBool());
            emit dataChanged(index, index);
            return true;
        }
        break;
    }
    return false;
}

bool StoryItemModel::insertRows(int position, int count, const QModelIndex &parent)
{
    StoryItem *parentItem = parent.isValid()?getItem(parent):rootItem;

    beginInsertRows(parent, position, position + count - 1);

    for (int row = 0; row < count; ++row) {
        StoryItem *newItem = new StoryItem(tr("New Task"));
        if (!parentItem->insertChild(position, newItem))
            break;
    }

    endInsertRows();
    return true;
}

bool StoryItemModel::removeRows(int position, int count, const QModelIndex &parent)
{
    StoryItem *parentItem = parent.isValid()?getItem(parent):rootItem;

    beginRemoveRows(parent, position, position + count - 1);
    for (int row = 0; row < count; ++row) {
        delete parentItem->takeChild(position);
    }
    endRemoveRows();

    return true;
}

QModelIndex StoryItemModel::index(int row, int column, const QModelIndex &parent) const
{
    StoryItem *parentItem;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = getItem(parent);

    StoryItem *childItem = parentItem->childAt(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}


QModelIndex StoryItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    StoryItem *childItem = getItem(index);
    return parent(childItem);
}

QModelIndex StoryItemModel::parent(const StoryItem *childItem) const
{
    StoryItem *parentItem = childItem->parent();

    if (childItem == rootItem || parentItem == rootItem || parentItem == 0)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}


// end model/view methods <<<
const QString MimeType = "application/pic";
QStringList StoryItemModel::mimeTypes() const
{
    QStringList types;
    types << MimeType;
    return types;
}

Qt::DropActions StoryItemModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::DropActions StoryItemModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

QMimeData *StoryItemModel::mimeData(const QModelIndexList &indexes) const
{

    if (indexes.count() !=1 )
        return 0;

    StoryItem *item = getItem(indexes.at(0));
    if (item) {
        QMimeData *mimeData = new QMimeData();
        QByteArray encodedData;
        QXmlStreamWriter writer(&encodedData);
        writeStoryAndChildren(&writer, item);
        mimeData->setData(MimeType, qCompress(encodedData));
        return mimeData;
    }
    return 0;

}

bool StoryItemModel::dropMimeData(const QMimeData *mimeData,
                                  Qt::DropAction action, int row, int /*column */,
                                  const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;
    if (action != Qt::MoveAction || !mimeData || !mimeData->hasFormat(MimeType))
        return false;
    StoryItem *parentItem = getItem(parent);
    if (parentItem) {
        QByteArray xmlData = qUncompress(mimeData->data(MimeType));
        QXmlStreamReader reader(xmlData);
        if (row == -1)
            row = parent.isValid() ? parentItem->childCount() : rootItem->childCount();
        beginInsertRows(parent, row, row);
        readStories(&reader, parentItem);
        endInsertRows();
        return true;
    }
    return false;
}


QModelIndex StoryItemModel::addStory(const QModelIndex &parent, StoryItem *newStoryItem)
{
    StoryItem *parentItem = getItem(parent);
    newStoryItem->setParent(parentItem);
    int position = parentItem->childCount();
    beginInsertRows(parent, position, position);
    endInsertRows();
    return index(position, 0, parent);
}

QModelIndex StoryItemModel::addStory(const QModelIndex &parent, const QVariantMap &map)
{
    StoryItem *parentItem = getItem(parent);
    int position = parentItem->childCount();
    beginInsertRows(parent, position, position);
    StoryItem *newStoryItem = new StoryItem(map, parentItem);
    endInsertRows();
    return index(position, 0, parent);
}


void StoryItemModel::fromList(const QVariantList &list)
{
    emit layoutAboutToBeChanged();

    //create lookup map with id and StoryItem
    QMap <int, StoryItem*> lookupMap;
    for (int i=0; i<list.count(); ++i) {
        QVariantMap map = list[i].toMap();
        StoryItem *story = new StoryItem(map);
        lookupMap.insert(story->data(ST_ID).toInt(), story);
    }
    //build tree
    for (QMap <int, StoryItem*>::iterator itr = lookupMap.begin(); itr != lookupMap.end(); ++itr) {
        StoryItem *item = itr.value();
        int parentId = itr.value()->data(ST_PARENT).toInt();
        StoryItem *parentItem;

        if (parentId == 0)
            parentItem = rootItem;
        else
            parentItem = lookupMap[parentId];

        item->setParent(parentItem);
    }

    emit layoutChanged();
}

void StoryItemModel::clear()
{
    //remove all rows from the storylist and notify attached views
    beginResetModel();
    rootItem->clear();
    endResetModel();
}

StoryItem *StoryItemModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        StoryItem *item = static_cast<StoryItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

const QString StoryTag("Story");
void StoryItemModel::writeStoryAndChildren(QXmlStreamWriter *writer, StoryItem *story) const
{
    if (story != rootItem) {
        writer->writeStartElement(StoryTag);
        story->writeStory(writer);
    }
    for (int i=0; i<story->childCount(); ++i)
        writeStoryAndChildren(writer, story->childAt(i));
    if (story != rootItem)
        writer->writeEndElement();
}

void StoryItemModel::readStories(QXmlStreamReader *reader, StoryItem *story)
{
    while (!reader->atEnd()) {
        reader->readNext();
        if (reader->isStartElement()) {
            if (reader->name() == StoryTag) {
                story = new StoryItem(reader, story);
                story->setData(ST_MODIFIED, true);
            }
        }
        else if (reader->isEndElement()) {
            if (reader->name() == StoryTag) {
                Q_ASSERT(story);
                story = story->parent();
                Q_ASSERT(story);
            }
        }
    }
}
