#ifndef TICKETMODEL_H
#define TICKETMODEL_H

#include <QAbstractTableModel>
#include "ticketdata.h"

typedef struct {TicketData ticketData; int printFlag;} ModelData;

class TicketModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TicketModel(QObject *parent = 0);
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    QVariant data (int row, int col, int role = Qt::DisplayRole ) const;
    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
    Qt::ItemFlags	flags ( const QModelIndex & index ) const;
    void sort ( int column, Qt::SortOrder order = Qt::AscendingOrder );
    void addTicket(const TicketData &t);
    void clear();

signals:
    
public slots:

private:
    QList<ModelData> theList;
    
};

#endif // TICKETMODEL_H
