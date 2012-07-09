#ifndef STORYITEMMODEL_H
#define STORYITEMMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

 class StoryItem;

 class StoryItemModel : public QAbstractItemModel
 {
     Q_OBJECT

 public:
     StoryItemModel(QObject *parent);
     ~StoryItemModel();

     QVariant data(const QModelIndex &index, int role) const;
     Qt::ItemFlags flags(const QModelIndex &index) const;
     QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;
     QModelIndex index(int row, int column,
                       const QModelIndex &parent = QModelIndex()) const;
     QModelIndex parent(const QModelIndex &index) const;
     int rowCount(const QModelIndex &parent = QModelIndex()) const;
     int columnCount(const QModelIndex &parent = QModelIndex()) const;

     void setupModelData(const QVariantList &list);

 private:
     StoryItem *rootItem;
 };

#endif // STORYITEMMODEL_H
