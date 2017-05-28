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
    enabled_(true)
{
    setZValue(1000);
    box_.setWidth(115.0);
    update_bounding_rect();
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

    if (!chart())
        return;

    // getting values
    std::vector<std::pair<QAbstractSeries*,QString>> vals;

    SetType x_val = chart()->mapToValue(chart()->mapFromScene(x(),0)).x();

    for(auto func : plot_->children())
    {
        auto func_item = dynamic_cast<FunctionItem*>(func);
        auto res = func_item->value()->values_at(x_val);

        for (auto rit=res.cbegin(); rit != res.cend(); ++rit){
            vals.push_back(std::make_pair(func_item->series(),QString::number(*rit,'g',6)));
        }
    }

    qreal ymiddle = chart()->mapToScene(chart()->plotArea().center()).y();
    box_.moveTop(ymiddle - box_.height()/2);
    box_.setHeight(5+3+marker_entry_height_*vals.size());

    auto plot_area_scene = chart()->mapToScene(chart()->plotArea()).boundingRect();
    auto clip_rect = this->mapFromScene(plot_area_scene).boundingRect();
    painter->setClipRect(clip_rect);

    QFont   font_bak = painter->font();
    QPen    marker_outline(Qt::black,0.5);
    QFont   marker_font("Monospace",marker_font_size_);
    QPen    marker_value(option->palette.light().color());

    // broom vertical line
    painter->setRenderHint(QPainter::Antialiasing, false);
    QRectF rect(chart()->plotArea());
    QPointF bottom(0,rect.bottom());
    QPointF top(0,rect.top());
    painter->drawLine(bottom,top);

    if (vals.empty())
        return;

    // broom rect text field
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    path.addRoundedRect(box_,3,3);
    painter->fillPath(path,QColor(0,0,0,144));

    QPointF pos = box_.topLeft() + QPointF(5,7);

    painter->setFont(marker_font);

    for (const auto& val: vals){
        auto markers = chart()->legend()->markers();
        auto marker_it = std::find_if(markers.cbegin(), markers.cend(), [&val](const auto& m){
            return m->series() == val.first;
        });

        if (marker_it == markers.cend()){
            qDebug() << "Marker for series not found.";
            continue;
        }

        QRectF marker_rect(pos,QSizeF(marker_size_,marker_size_));
        painter->fillRect(marker_rect,(*marker_it)->brush());

        painter->setRenderHint(QPainter::Antialiasing, false);
        painter->setPen(marker_outline);
        painter->drawRect(marker_rect);

        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setPen(marker_value);

        painter->drawText(pos + QPointF(marker_size_+5,marker_size_), val.second);

        /*QPointF cch = chart()->mapToPosition(QPointF(x_val, val.second.toDouble()));
        QPointF c = chart()->mapToScene(cch);
        qDebug() << c;

        painter->setPen(QPen(Qt::black,2));
        painter->drawEllipse(c, 5, 5);*/

        pos += QPointF(0,marker_entry_height_);

    }

    painter->setFont(font_bak);
}

void Broom::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    update_bounding_rect();
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
