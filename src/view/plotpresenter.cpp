#include "plotpresenter.h"

#include <algorithm>
#include <sstream>

#include <QGraphicsLayout>
#include <QBoxLayout>
#include <QStyle>
#include <QLineSeries>
#include <QValueAxis>
#include <QLogValueAxis>
#include <QDebug>
#include <QPushButton>
#include <QGraphicsProxyWidget>

PlotPresenter::PlotPresenter(QWidget* parent) :
    QGraphicsView(new QGraphicsScene, parent)
    //QChartView(parent)
{
    broom_group_ = new QGraphicsItemGroup;
    broom_group_->addToGroup(broom_line_ = new QGraphicsLineItem);
    /*broom_group_->addToGroup(broom_rect_ = new QGraphicsRectItem);*/
    broom_group_->addToGroup(broom_text_ = new QGraphicsSimpleTextItem);

    scene()->addItem(broom_group_);
    broom_group_->setZValue(1000);
    chart_ = nullptr;

    setRenderHint(QPainter::Antialiasing);
    setContentsMargins(0,0,0,0);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setMouseTracking(true);

    // setting up menu
    auto left_axis_action  = menu_.addAction(tr("Oś Y lewa"));
    auto right_axis_action = menu_.addAction(tr("Oś Y prawa"));
    menu_.setEnabled(false);

    QObject::connect(left_axis_action,SIGNAL(triggered()),
                     this, SLOT(change_left_axis()));
    QObject::connect(right_axis_action, SIGNAL(triggered()),
                     this, SLOT(change_right_axis()));
    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
                     this, SLOT(context_menu(QPoint)));
}

PlotPresenter::~PlotPresenter() {}

void PlotPresenter::show_plot(Plot *plot)
{
    if (chart_)
        this->scene()->removeItem(chart_);

    chart_ = nullptr;
    // looking for plot in internal storage
    auto it = std::find_if(plots_.cbegin(), plots_.cend(),
                 [plot](const PlotAdapterPtr& plot_ptr){ return plot_ptr->plot() == plot; });
    if (it != plots_.end())
    {
        chart_ = (*it)->chart();
    }
    else
    {
        plots_.emplace_back(PlotAdapterPtr(new PlotAdapter(plot)));
        chart_ = plots_.back()->chart();
    }

    chart_->resize(this->size());
    chart_->setAcceptHoverEvents(true);

    scene()->addItem(chart_);
    menu_.setEnabled(true);
}

void PlotPresenter::set_log_axis()
{
    QList<QAbstractAxis*> axes = chart_->axes(Qt::Vertical);
    auto old_axis = axes.first();

    QLogValueAxis* axisY = new QLogValueAxis;
    axisY->setLabelFormat("%g");
    axisY->setTitleText(old_axis->titleText());
    axisY->setBase(10);

    chart_->addAxis(axisY,Qt::AlignLeft);

    QList<QAbstractSeries*> series = chart_->series();
    for (auto& s : series)
    {
        for ( auto& a : s->attachedAxes())
        {
            if (a == old_axis)
            {
                s->detachAxis(old_axis);
                s->attachAxis(axisY);
            }
        }
    }

    chart_->removeAxis(old_axis);
}

void PlotPresenter::set_linear_axis()
{
    QList<QAbstractAxis*> axes = chart_->axes(Qt::Vertical);
    auto old_axis = axes.first();

    QValueAxis* axisY = new QValueAxis;
    axisY->setTitleText(old_axis->titleText());

    chart_->addAxis(axisY,Qt::AlignLeft);

    QList<QAbstractSeries*> series = chart_->series();
    for (auto& s : series)
    {
        for ( auto& a : s->attachedAxes())
        {
            if (a == old_axis)
            {
                s->detachAxis(old_axis);
                s->attachAxis(axisY);
            }
        }
    }

    chart_->removeAxis(old_axis);
}


Plot* PlotPresenter::plot() const
{
    return plot_;
}

void PlotPresenter::update_plot_cache(Plot *plot)
{
    // looking for plot in internal storage
    auto it = std::find_if(plots_.cbegin(), plots_.cend(),
                 [plot](const PlotAdapterPtr& plot_ptr){ return plot_ptr->plot() == plot; });
    if (it != plots_.end())
        (*it)->update();
    else
        plots_.emplace_back(PlotAdapterPtr(new PlotAdapter(plot)));
}

void PlotPresenter::remove_plot_cache(Plot* plot)
{
    // looking for plot in internal storage
    auto it = std::find_if(plots_.cbegin(), plots_.cend(),
                 [plot](const PlotAdapterPtr& plot_ptr){ return plot_ptr->plot() == plot; });
    if (it != plots_.end())
        plots_.erase(it);
}

void PlotPresenter::resizeEvent(QResizeEvent *event)
{
    if (chart_)
    {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        chart_->resize(event->size());

        QRectF rect = chart_->plotArea();
        broom_line_->setLine(0,rect.bottom(),0,rect.top());
        broom_text_->setY(rect.bottom()-rect.height()/2);
    }
    QGraphicsView::resizeEvent(event);
}

void PlotPresenter::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
        drag_start_ = event->pos();
}

//void PlotPresenter::adjusted_range(QVariant)

void PlotPresenter::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        //chart_->axisX()->setRange();
    }
}

void PlotPresenter::mouseMoveEvent(QMouseEvent *event)
{
    if (chart_)
    {
        if (chart_->plotArea().contains(event->pos()))
        {
            broom_group_->setPos(event->pos().x(), broom_group_->pos().y());
            broom_group_->show();
            broom_text_->setY(event->pos().y());
            if (chart_->plotArea().adjusted(0,0,-chart_->plotArea().width()/2,0).contains(event->pos()))
                broom_text_->setX(20);
            else
                broom_text_->setX(-broom_text_->boundingRect().width()-20);
            // setting value
            std::stringstream ss;
            bool domain_set = false;
            for (auto& s : chart_->series())
            {

                for (auto& a: chart_->axes())
                {
                }
                ss << s->name().toStdString() << " ";
            }
        }
        else
            broom_group_->hide();

        if (event->buttons() & Qt::LeftButton)
        {
            //event->
        }
    }

    QGraphicsView::mouseMoveEvent(event);
}

std::vector<QAbstractAxis*> PlotPresenter::get_y_axes() const
{
    if (chart_)
        return chart_->axes(Qt::Vertical).toVector().toStdVector();
    else
        return std::vector<QAbstractAxis*>();
}

void PlotPresenter::context_menu(const QPoint& point)
{
    if (menu_.isEnabled())
        menu_.exec(this->mapToGlobal(point));
}

void PlotPresenter::change_left_axis()
{

}

void PlotPresenter::change_right_axis()
{

}
