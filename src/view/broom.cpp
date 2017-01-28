#include "broom.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

ac::Broom::Broom(QChart* parent) : QGraphicsItem(parent), chart_(parent)
{
    posx_ = 50;
}

void ac::Broom::set_position(double pos)
{
    posx_ = pos;
}

QRectF ac::Broom::boundingRect() const
{
    QRectF rect(chart_->plotArea());
    rect.setLeft(posx_);
    rect.setRight(posx_);
}

void ac::Broom::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    QRectF rect(chart_->plotArea());
    QPointF bottom(posx_,rect.bottom());
    QPointF top(posx_,rect.top());
    painter->drawLine(bottom,top);
}

void ac::Broom::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    set_position(event->pos().x());
}
