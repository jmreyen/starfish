#ifndef STORYCARDSCENE_H
#define STORYCARDSCENE_H

#include <QGraphicsScene>
#include <QRectF>

class StoryCardScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit StoryCardScene(QObject *parent = 0);
    StoryCardScene(const QRectF &sceneRect, QObject *parent = 0);
    StoryCardScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = 0);
    virtual ~StoryCardScene();
signals:
    
public slots:
    void setID(const QString & text);
    void setDesc(const QString & text);
    void setNotes(const QString & text);
    void setHTD(const QString & text);
    void setImp(const QString & text);
    void setEst(const QString & text);
    void setUser(const QString & text);
    void setParent(const QString & text);
    void clearCard();

private slots:
    void initCard(qreal x, qreal y, qreal width, qreal height);

protected:
    void initDefaultLayout(qreal x, qreal y, qreal width, qreal height);

private:
    QGraphicsTextItem *theStoryID;
    QGraphicsTextItem *theStoryDesc;
    QGraphicsTextItem *theStoryNotes;
    QGraphicsTextItem *theStoryHTD;
    QGraphicsTextItem *theStoryImp;
    QGraphicsTextItem *theStoryEst;
    QGraphicsTextItem *theStoryUser;
    QGraphicsTextItem *theParentStory;

    QRectF theIDRect;
    QRectF theDescRect;
    QRectF theNotesRect;
    QRectF theHTDRect;
    QRectF theImpRect;
    QRectF theEstRect;
    QRectF theUserRect;
    QRectF theParentRect;

};

#endif // STORYCARDSCENE_H
