#include <QtGui>

#include "storyitem.h"
#include "storyitemmodel.h"

 StoryItemModel::StoryItemModel(/*const QString &data, */QObject *parent)
     : QAbstractItemModel(parent)
 {
     QList<QVariant> rootData;
     for (int i=ST_ID; i<ST_LAST; ++i)
         rootData << storyDisplayNames[i];
     rootItem = new StoryItem(rootData);
     //setupModelData(data.split(QString("\n")), rootItem);
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
         if (index.column()<ST_LAST)
             return item->data(index.column());
         break;
     case Qt::CheckStateRole:
         //Get status of the checkbox in the print column
         if (index.column()==ST_FLAG1)
             return item->printFlag();
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
     case Qt::CheckStateRole :
         if (index.column()==ST_FLAG1) {
             //Set status of the checkbox in the print column
             item->setPrintFlag(value.toInt());
             emit dataChanged(index, index);
             return true;
         }
         break;
     case Qt::EditRole:
         if (index.column() < ST_LAST) {
             item->setData(index.column(), value.toString());
             emit dataChanged(index, index);
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
     if (index.column()==ST_FLAG1)
         f = f | Qt::ItemIsUserCheckable;

     return f;
 }

 QVariant StoryItemModel::headerData(int section, Qt::Orientation orientation,
                                int role) const
 {
     if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
         return rootItem->data(section);

     return QVariant();
 }

 QModelIndex StoryItemModel::index(int row, int column, const QModelIndex &parent)
             const
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
     for (int i=0; i<list.count(); ++i) {
         QVariantMap map = list[i].toMap();
         StoryItem *story = new StoryItem(map, rootItem);
         rootItem->appendChild(story);
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

