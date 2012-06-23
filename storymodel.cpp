#include "storymodel.h"

#include <QIcon>
#include <QtAlgorithms>

StoryModel::StoryModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}


int StoryModel::rowCount ( const QModelIndex & parent ) const
{
    return theList.count();
}

int StoryModel::columnCount ( const QModelIndex & parent ) const
{
    return 10;
}

QVariant StoryModel::data ( const QModelIndex & index, int role) const
{
    return data(index.row(), index.column(), role);
}


QVariant StoryModel::data (int row, int col, int role) const
{
    const StoryData &t = theList[row].storyData;

    switch (role) {
    case Qt::EditRole:
    case Qt::DisplayRole:
        if (col<ST_LAST)
            return t[col];
        break;
    case Qt::CheckStateRole:
        //Get status of the checkbox in the print column
        if (col==9) return theList[row].printFlag;
        break;
    }
    return QVariant();
}


bool StoryModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
    //Set status of the checkbox in the print column
    if (role == Qt::CheckStateRole || index.column()==9) {
        theList[index.row()].printFlag = value.toInt();
        emit dataChanged(index, index);
        return true;
    }
    return false;

}
Qt::ItemFlags StoryModel::flags ( const QModelIndex & index ) const
{
    //Get flags from base class
    Qt::ItemFlags f = QAbstractTableModel::flags(index);
    //make item checkable if column = the print column
    if (index.column()==9)
        f = f | Qt::ItemIsUserCheckable;

    return f;
}

QVariant StoryModel::headerData ( int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        switch(role) {
        case Qt::DisplayRole:
            //retrun header strings
            switch (section)
            {
            case 0: return "ID";
            case 1: return "Description";
            case 2: return "Notes";
            case 3: return "How to Demo";
            case 4: return "Importance";
            case 5: return "Estimation";
            case 6: return "User";
            case 7: return "Sprint";
            case 8: return "Status";
            }
            break;
        case Qt::DecorationRole:
            switch (section) {
            case 9: return QIcon("c:/work/pic/printer.png");
            }
            break;
        }
    }
    return QVariant();
}

static int sortColumn = 0;

bool lt(const ModelData &t1, const ModelData &t2)
{
    return t1.storyData[sortColumn] < t2.storyData[sortColumn];
}

bool gt(const ModelData &t1, const ModelData &t2)
{
    return t1.storyData[sortColumn] > t2.storyData[sortColumn];
}

void StoryModel::sort ( int column, Qt::SortOrder order )
{
    sortColumn = column;
    if (order==Qt::AscendingOrder)
    {
        qSort(theList.begin(), theList.end(), lt);
    }
    else
    {
        qSort(theList.begin(), theList.end(), gt);
    }
    emit layoutChanged();
}

void StoryModel::addTicket(const StoryData &t)
{
    //add a row to the storylist and notify attached views
    ModelData m;
    m.storyData = t;
    m.printFlag = false;
    beginInsertRows(QModelIndex(), theList.count(), theList.count()+1);
    theList.append(m);
    endInsertRows();
}

void StoryModel::clear()
{
    //remove all rows from the storylist and notify attached views
    beginResetModel();
    theList.clear();
    endResetModel();
}