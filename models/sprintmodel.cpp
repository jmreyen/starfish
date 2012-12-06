#include "models/sprintmodel.h"
#include "base/fields.h"

#include <QListWidget>


void SprintModelItemDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const
{
    if (index.column() == SP_BURN) {
        QListWidget *w = static_cast<QListWidget *>(editor);
        QVariantList list = index.data().toList();
        int row = 0;
        foreach (QVariant v, list) {
            w->item(row++)->setText(v.toString());
        }
    }
    else
        QItemDelegate::setEditorData(editor, index);
}
void SprintModelItemDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
    if (index.column() == SP_BURN) {
        QListWidget *w = static_cast<QListWidget *>(editor);
        QVariantList list;
        for (int i=0; i<w->count(); ++i)
            list.append(w->item(i)->text().toInt());
        model->setData(index, list, Qt::EditRole);

    }
    else
        QItemDelegate::setModelData(editor, model, index);
}

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
    return SP_LAST;
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
        return t.data(col);
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

void SprintModel::fromList(const QVariantList &list)
{
    emit layoutAboutToBeChanged();
    foreach (QVariant v, list) {
        QVariantMap map = v.toMap();
        SprintData d(map);
        theList.append(d);
    }
    emit layoutChanged();
}

void SprintModel::clear()
{
    beginResetModel();
    theList.clear();
    endResetModel();
}

bool gt(const SprintData &d1, const SprintData &d2)
{
    return d1.dueDate() > d2.dueDate();
}

void SprintModel::sortByDate()
{
    beginResetModel();
    qSort(theList.begin(), theList.end(), gt);
    endResetModel();
}

SprintData SprintModel::sprint(int row) const
{
    if (row < theList.count())
        return theList[row];
    else
        return SprintData();
}



bool SprintModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    SprintData &s = theList[index.row()];
    if (role == Qt::EditRole ) {
        s.setData(index.column(), value);
        emit dataChanged(index, index);
        return true;
    }
    return false;
}


