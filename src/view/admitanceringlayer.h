#ifndef LAYER_H
#define LAYER_H

#include <QGraphicsItem>
#include <QResizeEvent>

#include "layer.h"

class FunctionItem;

class AdmitanceRingLayer : public Layer
{
public:
    AdmitanceRingLayer(FunctionItem* func);

    //QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget) override;
    //QRectF boundingRect() const;

    void resize(QSize size);
    void recalc();

private:
    qreal distance(QVector2D n, QVector2D t, QVector2D p);

    QPointF f0pos_;
    qreal f0_;
    QPointF middle_;
};

#endif // LAYER_H
