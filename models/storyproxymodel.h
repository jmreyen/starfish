#ifndef STORYPROXYMODEL_H
#define STORYPROXYMODEL_H

#include <QSortFilterProxyModel>

class StoryProxyModel : public QSortFilterProxyModel
{
Q_OBJECT
public:
    explicit StoryProxyModel(QObject *parent = 0);

signals:

public slots:

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    bool filterAcceptsRowItself(int source_row, const QModelIndex &source_parent) const;
    bool hasAcceptedChildren(int source_row, const QModelIndex &source_parent) const;

};

#endif // STORYPROXYMODEL_H
