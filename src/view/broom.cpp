#include "broom.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

Broom::Broom(QGraphicsItem *parent) : QGraphicsItem(parent), chart_(nullptr)
{
    posx_ = 50;
}

void Broom::set_position(double pos)
{
    posx_ = pos;
}

void Broom::set_chart(QChart *chart)
{
    chart_ = chart;
}

QRectF Broom::boundingRect() const
{
    if (chart_)
    {
        QRectF rect(chart_->plotArea());
        rect.setLeft(posx_-1);
        rect.setRight(posx_+1);
    }

    return QRectF();

}

void Broom::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    if (chart_)
    {
        QRectF rect(chart_->plotArea());
        QPointF bottom(posx_,rect.bottom());
        QPointF top(posx_,rect.top());
        painter->drawLine(bottom,top);
    }
}

void Broom::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    set_position(event->pos().x());
    update_geometry();
}

void Broom::update_geometry()
{
    prepareGeometryChange();
}
