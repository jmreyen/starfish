#include "burndownscene.h"
#include <QDebug>
#include <QGraphicsItem>

BurnDownScene::BurnDownScene(QObject *parent) :
    QGraphicsScene(parent)
{
}

BurnDownScene::BurnDownScene(const QRectF &sceneRect, QObject *parent) :
    QGraphicsScene(sceneRect, parent)
{
}

BurnDownScene::BurnDownScene(qreal x, qreal y, qreal width, qreal height, QObject *parent) :
    QGraphicsScene(x,y,width,height, parent)
{
}

BurnDownScene::~BurnDownScene()
{
}

int maxValue(const QList<int> &list)
{
    int max = 0;
    for (QList<int>::const_iterator i = list.begin(); i != list.end(); ++i)
        if (*i>max)
            max=*i;
    return max;
}

void BurnDownScene::show(int cap, int workDays, const QList<int> & list)
{
    clear();
    QRectF rect = displayRect(20);
    float dx = rect.width()/(workDays+1);
    float dy = rect.height()/cap;

    addLine(rect.x(), rect.height(), rect.x(), rect.y());
    addLine(rect.x(), rect.height(), rect.width(), rect.height());
    for (int i=1; i<=workDays;++i)
    {
        addLine(rect.x()+i*dx, rect.height(), rect.x()+i*dx, rect.height()+2 );
        QGraphicsTextItem *item = addText(QString::number(i));
        item->setPos(rect.x()+i*dx-5, rect.height());
    }

    QPointF pt(rect.x(), rect.y()+rect.height()-cap*dy);
    for (QList<int>::const_iterator i = list.begin(); i != list.end(); ++i)  {
        QPointF prevPt = pt;
        pt += QPointF(dx, dy**i);
        QLineF lf(prevPt, pt);
        addLine(lf);
        QGraphicsEllipseItem *e = addEllipse(pt.x(),pt.y(),3,3, QPen(Qt::black), QBrush(Qt::black));
        QGraphicsTextItem *item = addText(QString("-")+QString::number(*i));
        item->setPos(pt.x()-5, pt.y()-20);
    }

}

QRectF BurnDownScene::displayRect(int margin)
{
    QRectF rect = sceneRect();
    QRectF newRect;
    newRect.setTopLeft(QPoint(margin,margin));
    newRect.setBottomRight(QPoint(rect.width(),rect.height()));
    return newRect;
}
