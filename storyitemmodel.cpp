#include <QtGui>

#include "storyitem.h"
#include "storyitemmodel.h"



StoryIterator::StoryIterator(const StoryItemModel *model):
    theModel(model)

{
    makeFlatList(theModel->rootItem);
    if (theFlatList.count() == 0)
        current = -1;
    else
        current = 0;
}

void StoryIterator::makeFlatList(StoryItem *item)
{
    for (int i = 0; i<item->childCount(); ++i) {
        theFlatList.append(QPair<StoryItem *, int>(item->child(i), i));
        makeFlatList(item->child(i));
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

bool StoryIterator::operator !=(const StoryIterator &iterator) const
{
    return current!=iterator.current;
}

QModelIndex StoryIterator::currentIndex() const
{
    if (current == -1)
        return QModelIndex();

    const StoryItem *item = theFlatList[current].first;
    return theModel->createIndex(theFlatList[current].second, 0, item->parent());
}


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

 StoryItem *StoryItemModel::getItem(const QModelIndex &index) const
 {
     if (index.isValid()) {
         StoryItem *item = static_cast<StoryItem*>(index.internalPointer());
         if (item)
             return item;
         else
             qDebug() << "";
     }
     return rootItem;
 }

 int StoryItemModel::columnCount(const QModelIndex &parent) const
 {
    return getItem(parent)->columnCount();
 }

 QVariant StoryItemModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     StoryItem *item = getItem(index);

     switch (role) {
     case Qt::EditRole:
     case Qt::DisplayRole:
         if (index.column()<ST_PRINT)
             return item->data(index.column());
         break;
     case Qt::CheckStateRole:
         //Get status of the checkbox in the print column
         if (index.column()== ST_PRINT)
            return item->data(index.column()).toBool();
         break;
     }
     return QVariant();
 }

 bool StoryItemModel::setData ( const QModelIndex & index, const QVariant & value, int role )
 {
     if (!index.isValid())
         return false;

     StoryItem *item = getItem(index);

     switch (role){
     case Qt::EditRole:
         if (index.column() < ST_PRINT) {
             item->setData(index.column(), value.toString());
             emit dataChanged(index, index);
             return true;
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

 Qt::ItemFlags StoryItemModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return 0;

     Qt::ItemFlags f =Qt::ItemIsEnabled | Qt::ItemIsSelectable;
     if (index.column()==ST_PRINT)
         f = f | Qt::ItemIsUserCheckable;
     return f;
 }

 QVariant StoryItemModel::headerData(int section, Qt::Orientation orientation, int role) const
 {
     switch (role) {
     case Qt::DisplayRole:
         if (orientation == Qt::Horizontal && section != ST_PRINT)
                 return rootItem->data(section);
         break;
     case Qt::DecorationRole:
         if (orientation == Qt::Horizontal && section == ST_PRINT)
                 return QIcon(":images/printer.png");
         break;
     }
     return QVariant();
 }

 QModelIndex StoryItemModel::index(int row, int column, const QModelIndex &parent) const
 {
     if (!hasIndex(row, column, parent))
         return QModelIndex();

     StoryItem *parentItem = getItem(parent);
     StoryItem *childItem = parentItem->child(row);

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
     StoryItem *parentItem = childItem->parent();

     if (parentItem == rootItem || parentItem == 0)
         return QModelIndex();

     return createIndex(parentItem->row(), 0, parentItem);
 }

 int StoryItemModel::rowCount(const QModelIndex &parent) const
 {
     if (parent.column() > 0)
         return 0;

     return getItem(parent)->childCount();
 }

 QModelIndex StoryItemModel::addStory(const QModelIndex &parent, StoryItem *newStoryItem)
 {
     StoryItem *parentItem = getItem(parent);
     newStoryItem->setParent(parentItem);
     int position = parentItem->childCount();
     beginInsertRows(parent, position, position);
     parentItem->appendChild(newStoryItem);
     endInsertRows();
     return index(position, 0, parent);
 }

 QModelIndex StoryItemModel::addStory(const QModelIndex &parent, const QVariantMap &map)
 {
     StoryItem *parentItem = getItem(parent);
     StoryItem *newStoryItem = new StoryItem(map, parentItem);
     int position = parentItem->childCount();
     beginInsertRows(parent, position, position);
     parentItem->appendChild(newStoryItem);
     endInsertRows();
     return index(position, 0, parent);
 }


 void StoryItemModel::fromList(const QVariantList &list)
 {
     emit layoutAboutToBeChanged();

     //create lookup map with id and StoryItem
     QMap <QString, StoryItem*> lookupMap;
     for (int i=0; i<list.count(); ++i) {
         QVariantMap map = list[i].toMap();
         StoryItem *story = new StoryItem(map);
         lookupMap.insert(story->data(ST_ID).toString(), story);
     }
     //build tree
     for (QMap <QString, StoryItem*>::iterator itr = lookupMap.begin(); itr != lookupMap.end(); ++itr) {
         StoryItem *item = itr.value();
         QString parentId = itr.value()->data(ST_PARENT).toString();
         StoryItem *parentItem;

         if (parentId.isEmpty())
             parentItem = rootItem;
         else
             parentItem = lookupMap[parentId];

             item->setParent(parentItem);
             parentItem->appendChild(item);
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

