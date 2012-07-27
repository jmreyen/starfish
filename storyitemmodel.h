#ifndef STORYITEMMODEL_H
#define STORYITEMMODEL_H


#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include "storyitem.h"
#include "fields.h"

class StoryItem;

class StoryIterator
{
    friend class StoryItemModel;

public:
    StoryIterator() {current = -1;}
    StoryIterator &operator = (StoryIterator &iterator);
    StoryIterator &operator ++();
    const StoryItem *operator ->();
    bool operator !=(const StoryIterator &iterator) const;
protected:
    StoryIterator(StoryItem *item);
    void makeFlatList(StoryItem *item);
private:
    QList<StoryItem*> theFlatList;
    int current;
};

class StoryItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    StoryItemModel(QObject *parent);
    ~StoryItemModel();


    StoryItem *getItem(const QModelIndex &index) const;

    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    bool setData ( const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex addStory(const QModelIndex &parent, StoryItem *newStoryItem);
    QModelIndex addStory(const QModelIndex &parent, const QVariantMap &map);
    void fromList(const QVariantList &list);
    void clear();

    typedef StoryIterator iterator;
    iterator begin() const {StoryIterator itr(rootItem); return itr;}
    iterator end() const {return StoryIterator();}

private:
    StoryItem *rootItem;
};

#endif // STORYITEMMODEL_H
