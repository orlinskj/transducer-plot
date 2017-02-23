#include "broom.h"

#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QLegendMarker>
#include <QLineSeries>

#include <boost/optional.hpp>
#include "../viewmodel/functionitem.h"

Broom::Broom(QGraphicsItem *parent) :
    QGraphicsItem(parent),
    plot_(nullptr)
{
    box_.setSize(QSizeF(100.0,60.0));
    setPos(50,0);
    setZValue(1000);
}

void Broom::set_position(double x)
{
    this->setPos(QPointF(x, this->pos().y()));

    // switch box position [right/left to broom line]
    qreal xmiddle = chart()->plotArea().center().x();
    if(chart()->mapFromScene(x,0).x() < xmiddle)
    {
        if (chart()->mapFromScene(pos().x(),0).x() >= xmiddle)
            update();
        box_.moveLeft(box_offset_);
    }
    else
    {
        if (chart()->mapFromScene(pos().x(),0).x() < xmiddle)
            update();
        box_.moveRight(-box_offset_);
    }
}

void Broom::set_plot(PlotItem* plot)
{
    plot_ = plot;
    update();
}

QRectF Broom::boundingRect() const
{
    if (!chart())
        return QRectF();

    QRectF rect(chart()->plotArea());
    rect.setLeft( std::min(-1.0,box_.left()) );
    rect.setRight( std::max(1.0, box_.right()) );

    return rect;
}

void Broom::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!plot())
        return;

    QFont   font_bak = painter->font();
    QPen    marker_outline(Qt::black,0.5);
    QFont   marker_font("",marker_font_size_);
    QPen    marker_value(option->palette.light().color());

    painter->setRenderHint(QPainter::Antialiasing, false);
    QRectF rect(chart()->plotArea());
    QPointF bottom(0,rect.bottom());
    QPointF top(0,rect.top());
    painter->drawLine(bottom,top);

    painter->setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    path.addRoundedRect(box_,3,3);
    painter->fillPath(path,QColor(0,0,0,144));

    QPointF pos = box_.topLeft() + QPointF(5,7);

    painter->setFont(marker_font);

    for (auto marker: chart()->legend()->markers())
    {
        auto funcs = plot_->children();
        auto f_it = std::find_if(funcs.begin(), funcs.end(),
            [marker,this](const TreeItem* t){
                if(auto f = dynamic_cast<const FunctionItem*>(t))
                {
                    //qDebug() << marker->series();
                    //qDebug() << dynamic_cast<QLineSeries*>(marker->series());
                    return f->series() == dynamic_cast<QLineSeries*>(marker->series());
                }
                return false;
            });

        if (f_it == funcs.end())
            throw std::runtime_error("function with same series like marker not found");

        auto func_item = dynamic_cast<FunctionItem*>(*f_it);

        QRectF rect(pos,QSizeF(marker_size_,marker_size_));
        painter->fillRect(rect,marker->brush());

        painter->setRenderHint(QPainter::Antialiasing, false);
        painter->setPen(marker_outline);
        painter->drawRect(rect);

        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setPen(marker_value);

        SetType x_val = chart()->mapToValue(chart()->mapFromScene(x(),0)).x();
        QString text;
        auto val = func_item->value()->value_at(x_val);
        if (val)
            text = QString::number(*val);
        else
            text = "-";

        painter->drawText(pos + QPointF(marker_size_+5,marker_size_),text);

        pos += QPointF(0,marker_entry_height_);
    }

    painter->setFont(font_bak);
}

QChart* Broom::chart() const
{
    if (plot_)
        return plot_->chart();
    else
        return nullptr;
}

PlotItem* Broom::plot() const
{
    return plot_;
}

void Broom::update()
{
    prepareGeometryChange();
    if (!chart())
        return;

    qreal ymiddle = chart()->mapToScene(chart()->plotArea().center()).y();
    box_.moveTop(ymiddle - box_.height()/2);
    box_.setHeight(5+3+marker_entry_height_*chart()->series().count());
}
