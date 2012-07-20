#include "storyitemdelegate.h"
#include "fields.h"

#include <QComboBox>

StoryItemDelegate::StoryItemDelegate()
{
}

void StoryItemDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const
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
void StoryItemDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
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
