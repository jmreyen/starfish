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

 int StoryItemModel::columnCount(const QModelIndex &parent) const
 {
     if (parent.isValid())
         return static_cast<StoryItem*>(parent.internalPointer())->columnCount();
     else
         return rootItem->columnCount();
 }

 QVariant StoryItemModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     StoryItem *item = static_cast<StoryItem*>(index.internalPointer());

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

     StoryItem *item = static_cast<StoryItem*>(index.internalPointer());

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

     StoryItem *parentItem;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<StoryItem*>(parent.internalPointer());

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

     StoryItem *childItem = static_cast<StoryItem*>(index.internalPointer());
     StoryItem *parentItem = childItem->parent();

     if (parentItem == rootItem)
         return QModelIndex();

     return createIndex(parentItem->row(), 0, parentItem);
 }

 int StoryItemModel::rowCount(const QModelIndex &parent) const
 {
     StoryItem *parentItem;
     if (parent.column() > 0)
         return 0;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<StoryItem*>(parent.internalPointer());

     return parentItem->childCount();
 }


 void StoryItemModel::fromList(const QVariantList &list)
 {
     for (int i=0; i<list.count(); ++i) {
         QVariantMap map = list[i].toMap();
         StoryItem *story = new StoryItem(map, rootItem);
         rootItem->appendChild(story);
     }
 }

 void StoryItemModel::clear()
 {
     //remove all rows from the storylist and notify attached views
     beginResetModel();
     rootItem->clear();
     endResetModel();
 }

