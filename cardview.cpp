#include "cardview.h"

#include <qgraphicsscene>

CardView::CardView(QWidget *parent) :
    QGraphicsView(parent),
    theScene(new(QGraphicsScene))
{
}
CardView::~CardView()
{
    delete theScene;
}


void CardView::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    paint(painter);
    painter.end();
}

void CardView::paint(QPainter &painter)
{
    painter.setClipRect(QRect(0, 0, 200, 200));
    painter.fillRect(QRect(0, 0, 200, 200), Qt::gray);
    painter.setPen(QPen(Qt::white, 4, Qt::DashLine));
    painter.drawLine(QLine(0, 35, 200, 35));
    painter.drawLine(QLine(0, 165, 200, 165));

    painter.translate(100, 100);
}
