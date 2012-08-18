#ifndef STORYITEMMODEL_H
#define STORYITEMMODEL_H

#include "storyitem.h"
#include "fields.h"

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class QXmlStreamWriter;

class StoryItem;
class StoryItemModel;

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
    StoryItem *theRootItem;
    QList<QPair<StoryItem*, int> > theFlatList;
    int current;
};

class StoryItemModel : public QAbstractItemModel
{
    Q_OBJECT
    friend class StoryIterator;

public:
    StoryItemModel(QObject *parent);
    ~StoryItemModel();

    // inherited from QAbstractItemModel
    // All models
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    // Table and tree models
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    // Editable models
    bool setData ( const QModelIndex &index, const QVariant &value, int role);
    // All resizable models
    bool insertRows(int position, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int position, int count, const QModelIndex &parent);
    // Tree models
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    // drag and drop
    Qt::DropActions supportedDropActions() const;
    Qt::DropActions supportedDragActions() const;
    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *mimeData, Qt::DropAction action,
                      int row,int column, const QModelIndex &parent);


    QModelIndex addStory(const QModelIndex &parent, StoryItem *newStoryItem);
    QModelIndex addStory(const QModelIndex &parent, const QVariantMap &map);

    void writeStoryAndChildren(QXmlStreamWriter *writer, StoryItem *story) const;
    void readStories(QXmlStreamReader *reader, StoryItem *story);

    void fromList(const QVariantList &list);
    void clear();



    typedef StoryIterator iterator;
    iterator begin() const {StoryIterator itr(rootItem); return itr;}
    iterator end() const {return StoryIterator();}
protected:
    StoryItem *getItem(const QModelIndex &index) const;

private:
    StoryItem *rootItem;
};

#endif // STORYITEMMODEL_H
