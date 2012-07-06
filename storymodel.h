#ifndef STORYMODEL_H
#define STORYMODEL_H

#include "storydata.h"

#include <QAbstractTableModel>
#include <QItemDelegate>


class StoryModelItemDelegate : public QItemDelegate
{
public:
  void setEditorData ( QWidget * editor, const QModelIndex & index ) const;
  void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
};

typedef struct {StoryData storyData; int printFlag; int sortPos;} StoryModelData;

const int ST_FLAG1     = ST_LAST;
const int ST_MODELLAST = ST_LAST + 1;

class StoryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit StoryModel(QObject *parent = 0);
    int rowCount ( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount ( const QModelIndex &parent = QModelIndex() ) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    QVariant data ( const QModelIndex &index, int role = Qt::DisplayRole ) const;
    QVariant data (int row, int col, int role = Qt::DisplayRole ) const;
    bool setData ( const QModelIndex &index, const QVariant & value, int role = Qt::EditRole );
    Qt::ItemFlags	flags ( const QModelIndex &index ) const;
    void sort ( int column, Qt::SortOrder order = Qt::AscendingOrder );
    void addStory(const StoryData &t);
    void fromList(const QVariantList &list);
    void clear();
    const StoryData &story(int i) const {return theList[i].storyData;}
    int id(int row){return theList[row].storyData[ST_ID].toInt();}
    bool printFlag(int row) const {return theList[row].printFlag;}

signals:
    
public slots:

private:
    QList<StoryModelData> theList;
    
};

#endif // STORYMODEL_H
