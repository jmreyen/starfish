#ifndef STORYITEM_H
#define STORYITEM_H



extern const char *storyDisplayNames[];

class StoryItem
{
public:
    StoryItem(const QVariantList &list, StoryItem *parent = 0);
    StoryItem(const QVariantMap &map, StoryItem *parent = 0);
     ~StoryItem();

     void appendChild(StoryItem *child);

     StoryItem *child(int row);
     int childCount() const;
     int columnCount() const;
     QVariant data(int column) const;
     bool setData(int column, const QVariant &value);
     int row() const;
     StoryItem *parent() const;
     void setParent(StoryItem *parent);

     const StoryItem &fromMap(const QVariantMap &map);
     void clear();

 private:
     QList<StoryItem*> childItems;
     QList<QVariant> itemData;
     StoryItem *parentItem;
     bool thePrintFlag;
     int sortPosition;

};

#endif // STORYITEM_H
