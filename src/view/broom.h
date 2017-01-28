#ifndef BROOM_H
#define BROOM_H

#include <QGraphicsItem>
#include <QChart>

using namespace QtCharts;

namespace ac{

class Broom : public QGraphicsItem
{
public:
    Broom(QChart *parent);

    void set_position(double pos);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);

protected:
    //void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    QChart* chart_;
    double posx_;
};

}

#endif // BROOM_H
