#ifndef STORYITEMDELEGATE_H
#define STORYITEMDELEGATE_H

#include <QItemDelegate>

class StoryItemDelegate : public QItemDelegate
{
public:
    StoryItemDelegate();
    void setEditorData ( QWidget * editor, const QModelIndex & index ) const;
    void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
};

#endif // STORYITEMDELEGATE_H
