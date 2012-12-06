#ifndef SPRINTMODEL_H
#define SPRINTMODEL_H

#include "sprintdata.h"

#include <QAbstractTableModel>
#include <QItemDelegate>

class SprintModelItemDelegate : public QItemDelegate
{
public:
  void setEditorData ( QWidget * editor, const QModelIndex & index ) const;
  void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
};

class SprintModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SprintModel(QObject *parent = 0);
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    QVariant data (int row, int col, int role = Qt::DisplayRole ) const;
    SprintData sprint(int row) const;
    void addSprint(const SprintData &t);
    void fromList(const QVariantList &list);
    void sortByDate();
    void clear();
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
private:
    QList<SprintData> theList;
signals:
public slots:
};

#endif // SPRINTMODEL_H
