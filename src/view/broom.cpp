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
    plot_(nullptr),
    enabled_(true),
    bounding_rect_(QRectF())
{
    setZValue(1000);
    box_.setWidth(75.0);
}

void Broom::set_position(QPointF position, bool force)
{
    if ((!enabled_ && !force) || !chart())
        return;

    auto old_x = pos().x();
    auto new_x = position.x();
    this->setPos(QPointF(new_x, this->pos().y()));

    auto broom_chart_pos = chart()->mapFromScene(pos());
    x_value_ = chart()->mapToValue(broom_chart_pos).x();

    // switch box position [right/left to broom line]
    qreal xmiddle = chart()->plotArea().center().x();
    if(broom_chart_pos.x() < xmiddle)
    {
        box_.moveLeft(box_offset_);
        if (old_x >= xmiddle)
            update_bounding_rect();
    }
    else
    {
        box_.moveRight(-box_offset_);
        if (old_x < xmiddle)
            update_bounding_rect();
    }
}

void Broom::set_plot(PlotItem* plot)
{
    enabled_ = true;
    plot_ = plot;
    update_bounding_rect();
}

void Broom::update_bounding_rect()
{
    qDebug() << "update_bounding_rect()";
    prepareGeometryChange();
    if (chart())
    {
        bounding_rect_ = chart()->plotArea();
        bounding_rect_.setLeft( std::min(-1.0,box_.left()) );
        bounding_rect_.setRight( std::max(1.0, box_.right()) );
    }
    else
        bounding_rect_ = QRectF();
}

void Broom::update_position()
{
    if (chart())
        set_position(chart()->mapToScene(chart()->mapToPosition(QPointF(x_value_,0))),true);
}

QRectF Broom::boundingRect() const
{
    return bounding_rect_;
}

void Broom::toggle()
{
    enabled_ = !enabled_;
}

void Broom::set_visibility(bool visibility)
{
    if (enabled_)
        setVisible(visibility);
}

void Broom::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    qreal ymiddle = chart()->mapToScene(chart()->plotArea().center()).y();
    box_.moveTop(ymiddle - box_.height()/2);
    box_.setHeight(5+3+marker_entry_height_*chart()->series().count());

    if (!plot())
        return;

    auto plot_area_scene = chart()->mapToScene(chart()->plotArea()).boundingRect();
    auto clip_rect = this->mapFromScene(plot_area_scene).boundingRect();
    painter->setClipRect(clip_rect);

    QFont   font_bak = painter->font();
    QPen    marker_outline(Qt::black,0.5);
    QFont   marker_font("",marker_font_size_);
    QPen    marker_value(option->palette.light().color());

    // broom vertical line
    painter->setRenderHint(QPainter::Antialiasing, false);
    QRectF rect(chart()->plotArea());
    QPointF bottom(0,rect.bottom());
    QPointF top(0,rect.top());
    painter->drawLine(bottom,top);

    // broom rect text field
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

void Broom::resizeEvent(QResizeEvent* event)
{
    qDebug() << "resize event [BROOM]";
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
