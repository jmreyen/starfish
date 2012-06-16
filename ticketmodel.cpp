#include "ticketmodel.h"

TicketModel::TicketModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}


int TicketModel::rowCount ( const QModelIndex & parent ) const
{
    return theList.count();

}

int TicketModel::columnCount ( const QModelIndex & parent ) const
{
    return sizeof(TicketData);
}

QVariant TicketModel::data ( const QModelIndex & index, int role) const
{
    return data(index.row(), index.column(), role);
}


QVariant TicketModel::data (int row, int col, int role) const
{

    if (role == Qt::CheckStateRole && col==9)
        return printFlags[row];

    if (role != Qt::DisplayRole)
        return QVariant();   const TicketData &t = theList[row];

    switch (col){
        case 0: return t.theID;
        case 1: return t.theDesc;
        case 2: return t.theNotes;
        case 3: return t.theHTD;
        case 4: return t.theImp;
        case 5: return t.theEst;
        case 6: return t.theUser;
        case 7: return t.theSprint;
        case 8: return t.theStatus;
        default: return QVariant();
    }
}


bool TicketModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
    if (role == Qt::CheckStateRole || index.column()==9) {
        printFlags[index.row()] = value.toInt();
        emit dataChanged(index, index);
        return true;
    }
    return false;

}
Qt::ItemFlags	TicketModel::flags ( const QModelIndex & index ) const
{
    Qt::ItemFlags f = QAbstractTableModel::flags(index);

    if (index.column()==9)
        f = f | Qt::ItemIsUserCheckable;

    return f;
}

QVariant TicketModel::headerData ( int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

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
            case 9: return "P";
            default: return "";
    }
}

void TicketModel::addTicket(const TicketData &t)
{
    beginInsertRows(QModelIndex(), theList.count(), theList.count()+1);
    theList.append(t);
    printFlags.append(false);
    endInsertRows();
}

void TicketModel::clear()
{
    beginResetModel();
    theList.clear();
    endResetModel();
}
