#ifndef STORYITEM_H
#define STORYITEM_H


const int ST_ID     = 0;
const int ST_DESC   = 1;
const int ST_NOTES  = 2;
const int ST_HTD    = 3;
const int ST_IMP    = 4;
const int ST_EST    = 5;
const int ST_USER   = 6;
const int ST_TYP    = 7;
const int ST_STATUS = 8;
const int ST_SPRINT = 9;
const int ST_COMP   = 10;
const int ST_VERSION= 11;
const int ST_LAST   = 12;

extern const char *storyFieldNames[];
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
     StoryItem *parent();
     void setParent(StoryItem *parent);

     const StoryItem &fromMap(const QVariantMap &map);
     void clear();
     bool printFlag() const {return thePrintFlag;}
     void setPrintFlag(bool b) {thePrintFlag = b;}


 private:
     QList<StoryItem*> childItems;
     QList<QVariant> itemData;
     StoryItem *parentItem;
     bool thePrintFlag;
     int sortPosition;

};

#endif // STORYITEM_H
