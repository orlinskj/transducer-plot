#ifndef MINMAXLAYER_H
#define MINMAXLAYER_H

#include "layer.h"

class PlotItem;

/**
 * @brief The MinMaxLayer class
 * @desc Stores min-max range for every function. Item is being displayed in top-left corner of plot area.
 */
class MinMaxLayer : public Layer
{
public:
    MinMaxLayer(PlotItem* plot);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget) override;
    void resize(QSize size);
    void recalc();
};

#endif // MINMAXLAYER_H
