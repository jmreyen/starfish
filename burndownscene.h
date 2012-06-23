#ifndef BURNDOWNSCENE_H
#define BURNDOWNSCENE_H
#include <QGraphicsScene>


class BurnDownScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit BurnDownScene(QObject *parent = 0);
    BurnDownScene(const QRectF &sceneRect, QObject *parent = 0);
    BurnDownScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = 0);
    virtual ~BurnDownScene();
    void show(int cap, int workDays, const QList<int> & list);
    QRectF displayRect(int margin);

signals:
    
public slots:

private:
    QList<int>  theList;
    
};

#endif // BURNDOWNSCENE_H
