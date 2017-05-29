#ifndef MINMAXLAYER_H
#define MINMAXLAYER_H

#include "layer.h"

class PlotItem;

class MinMaxLayer : public Layer
{
public:
    MinMaxLayer(PlotItem* plot);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget) override;
    void resize(QSize size);
    void recalc();
};

#endif // MINMAXLAYER_H
