#include "sprintmodel.h"

SprintModel::SprintModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int SprintModel::rowCount ( const QModelIndex & parent ) const
{
    return theList.count();
}

int SprintModel::columnCount ( const QModelIndex & parent ) const
{
    return 1;
}

QVariant SprintModel::data ( const QModelIndex & index, int role) const
{
    return data(index.row(), index.column(), role);
}


QVariant SprintModel::data (int row, int col, int role) const
{
    const SprintData &t = theList[row];

    switch (role) {
    case Qt::EditRole:
    case Qt::DisplayRole:
        switch(col) {
            case 0: return t.name();
            case 1: return t.dueDate();
        }
        break;
    }
    return QVariant();
}

void SprintModel::addSprint(const SprintData &d)
{
    beginInsertRows(QModelIndex(), theList.count(), theList.count()+1);
    theList.append(d);
    endInsertRows();
}

void SprintModel::clear()
{
    beginResetModel();
    theList.clear();
    endResetModel();
}
