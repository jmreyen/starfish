#include "storymodel.h"

#include <QIcon>
#include <QtAlgorithms>
#include <QComboBox>

void StoryModelItemDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const
{
    switch (index.column()){
    case ST_IMP:
    case ST_EST:
    case ST_TYP:
    case ST_SPRINT:
    case ST_COMP:
    case ST_VERSION:
    {
        QComboBox *w = static_cast<QComboBox *>(editor);
        int n = w->findText(index.data().toString());
        w->setCurrentIndex(n);
        break;
    }
    default:
        QItemDelegate::setEditorData(editor, index);
        break;
    }
}
void StoryModelItemDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
    switch (index.column()){
    case ST_IMP:
    case ST_EST:
    case ST_TYP:
    case ST_SPRINT:
    case ST_COMP:
    case ST_VERSION:
    {
        QComboBox *w = static_cast<QComboBox *>(editor);
        QString str = w->currentText();
        model->setData(index, str, Qt::EditRole);
        break;
    }
    default:
        QItemDelegate::setModelData(editor, model, index);
        break;
    }
}

StoryModel::StoryModel(QObject *parent) :
    QAbstractTableModel(parent),
    sorted(false),
    sortColumn(ST_ID),
    sortOrder(Qt::DescendingOrder)
{
}


int StoryModel::rowCount ( const QModelIndex & parent ) const
{
    return theList.count();
}

int StoryModel::columnCount ( const QModelIndex & parent ) const
{
    return ST_MODELLAST;
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
        if (col==ST_FLAG1) return theList[row].printFlag;
        break;
    }
    return QVariant();
}


bool StoryModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{

    switch (role){
    case Qt::CheckStateRole :
        if (index.column()==ST_FLAG1) {
            //Set status of the checkbox in the print column
            theList[index.row()].printFlag = value.toInt();
            emit dataChanged(index, index);
            return true;
        }
        break;
    case Qt::EditRole:
        if (index.column() < ST_LAST) {
            theList[index.row()].storyData[index.column()] = value.toString();
            emit dataChanged(index, index);
        }
        break;
    }
    return false;
}
Qt::ItemFlags StoryModel::flags ( const QModelIndex & index ) const
{
    //Get flags from base class
    Qt::ItemFlags f = QAbstractTableModel::flags(index);
    //make item checkable if column = the print column
    if (index.column()==ST_FLAG1)
        f = f | Qt::ItemIsUserCheckable;

    return f;
}

QVariant StoryModel::headerData ( int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        switch(role) {
        case Qt::DisplayRole:
            //return header strings
            return storyDisplayNames[section];
        case Qt::DecorationRole:
            switch (section) {
            case ST_FLAG1: return QIcon("printer.png");
            }
            break;
        }
    }
    return QVariant();
}

static int tmpSortColumn = 0;

bool lt(const StoryModelData &t1, const StoryModelData &t2)
{
    if (tmpSortColumn  == ST_ID)
        return t1.storyData[tmpSortColumn ].toInt() < t2.storyData[tmpSortColumn ].toInt();
    else
        return t1.storyData[tmpSortColumn ] < t2.storyData[tmpSortColumn ];
}

bool gt(const StoryModelData &t1, const StoryModelData &t2)
{
    if (tmpSortColumn == ST_ID)
        return t1.storyData[tmpSortColumn ].toInt() > t2.storyData[tmpSortColumn ].toInt();
    else
        return t1.storyData[tmpSortColumn ] > t2.storyData[tmpSortColumn ];
}

void StoryModel::sort ( int column, Qt::SortOrder order )
{
    // invalid column - nothing to do
    if (column >= ST_LAST)
        return;
    // tell all views that the layout will change
    emit layoutAboutToBeChanged();
    //store the row numbers before sorting
    for (int i=0; i<theList.count(); ++i)
        theList[i].sortPos=i;
    //sort
    tmpSortColumn  = column;
    if (order==Qt::AscendingOrder)
        qSort(theList.begin(), theList.end(), lt);
    else
        qSort(theList.begin(), theList.end(), gt);
    //restore persistent model indices
    QModelIndexList fromIndexes;
    QModelIndexList toIndexes;
    for (int r = 0; r < rowCount(); ++r) {
        for (int c = 0; c < columnCount(); ++c) {
            toIndexes.append(createIndex(r, c, 0));
            fromIndexes.append(createIndex(theList[r].sortPos, c, 0));
        }
    }
    changePersistentIndexList(fromIndexes, toIndexes);
    // tell all attached views the the layout has been changesd
    emit layoutChanged();
    // remember the sort order
    sorted = true;
    sortColumn = column;
    sortOrder = order;
}

int StoryModel::addStory(const StoryData &d)
{
    //add a row to the storylist and notify attached views
    StoryModelData m;
    m.storyData = d;
    m.printFlag = false;
    beginInsertRows(QModelIndex(), theList.count(), theList.count()+1);
    theList.append(m);
    endInsertRows();
    // if the model was previously sorted ...
    if (sorted) {
        // ... sort it again and return new row number
        QPersistentModelIndex i = createIndex(theList.count()-1, ST_ID);
        sort(sortColumn, sortOrder);
        return i.row();
    }
    else
        return theList.count()-1;
}

void StoryModel::fromList(const QVariantList &list)
{
    emit layoutAboutToBeChanged();
    foreach (QVariant v, list) {
        QVariantMap map = v.toMap();
        StoryModelData m;
        m.storyData.fromMap(map);
        m.printFlag = false;
        theList.append(m);
    }
    emit layoutChanged();
}



void StoryModel::clear()
{
    //remove all rows from the storylist and notify attached views
    beginResetModel();
    theList.clear();
    endResetModel();
}
