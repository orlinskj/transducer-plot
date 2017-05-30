#include "minmaxlayer.h"

#include "../viewmodel/plotitem.h"
#include "../viewmodel/functionitem.h"
#include <QPainter>
#include <QLegendMarker>

MinMaxLayer::MinMaxLayer(PlotItem *plot) :
    Layer(plot,QObject::tr("Zakresy funkcji"))
{ }

void MinMaxLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!plot_ || !plot_->chart())
        return;

    QPointF cur = plot_->chart()->plotArea().topLeft() + QPointF(10,10);
    QSize marker_size(7,7);

    auto font = QFont("Monospace", 7);
    painter->setFont(font);

    for (auto func: plot_->children()){
        auto func_item = dynamic_cast<FunctionItem*>(func);

        auto brush = plot_->chart()->legend()->markers(func_item->series()).first()->brush();

        painter->fillRect(QRectF(cur, marker_size),brush);
        //painter->drawRect(QRectF(cur, marker_size));
        auto min = std::get<0>(func_item->value()->codomain()->min());
        auto max = std::get<0>(func_item->value()->codomain()->max());
        QString range = QString("(") + QString::number(min,'g',8) + QString("; ")
                + QString::number(max,'g',8) + QString(")");

        painter->drawText(cur+QPointF(12, 7), range);

        cur += QPointF(0,15);
    }
}

void MinMaxLayer::recalc()
{
    //
}

void MinMaxLayer::resize(QSize size)
{
    //
}
