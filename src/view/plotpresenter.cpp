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

PlotPresenter::PlotPresenter(QWidget* parent, PlotStoreItemModel *store) :
    QGraphicsView(new QGraphicsScene, parent),
    plot_(nullptr),
    store_(store),
    broom_(new Broom)
{
    broom_->setVisible(false);
    scene()->addItem(broom_);

    setRenderHint(QPainter::Antialiasing);
    setContentsMargins(0,0,0,0);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setMouseTracking(true);

    // setting up menu
    menu_.setEnabled(false);
    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
                     this, SLOT(context_menu(QPoint)));
}

PlotPresenter::~PlotPresenter() {}

void PlotPresenter::show_plot(PlotItem *plot)
{
    broom_->set_plot(plot);

    if (chart())
        this->scene()->removeItem(chart());

    plot_ = plot;

    scene()->addItem(plot_->chart());
    plot_->chart()->resize(this->size());
    plot_->chart()->setAcceptHoverEvents(true);

    alter_menu();
}

void PlotPresenter::alter_menu()
{
    if (!plot_->chart())
        return;

    menu_.setEnabled(true);
    menu_.clear();

    for (auto axis: plot_->chart()->axes(Qt::Vertical))
    {
        QString change_to("liniowa");
        if (dynamic_cast<QValueAxis*>(axis))
            change_to = "logarytmiczna";

        QString text(QString("\"")+axis->titleText() + "\" na: " + change_to);
        QAction* action = menu_.addAction(text);
        ///TODO add action here
        connect(action, &QAction::triggered,
                this, [axis,this](){ /*this->change_axis(axis);*/ });
    }
}

PlotItem* PlotPresenter::plot() const
{
    return plot_;
}

QChart* PlotPresenter::chart() const
{
    if (plot_)
        return plot_->chart();
    else
        return nullptr;
}

void PlotPresenter::resizeEvent(QResizeEvent *event)
{
    if (plot_->chart())
    {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        plot_->chart()->resize(event->size());
    }
    broom_->update();
    QGraphicsView::resizeEvent(event);
}

void PlotPresenter::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
        drag_start_ = event->pos();
}

void PlotPresenter::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        //chart_->axisX()->setRange();
    }
}

void PlotPresenter::mouseMoveEvent(QMouseEvent *event)
{
    if (plot_->chart())
    {
        // qDebug() << "mouseMove(" << event->pos() << ")";
        if (chart()->plotArea().contains(event->pos()))
        {
            broom_->show();
            broom_->set_position(event->pos().x());
        }
        else
        {
            broom_->hide();
        }

        if (event->buttons() & Qt::LeftButton)
        {
            //event->
        }
    }

    QGraphicsView::mouseMoveEvent(event);
}

void PlotPresenter::context_menu(const QPoint& point)
{
    if (menu_.isEnabled())
        menu_.exec(this->mapToGlobal(point));
}

/*void PlotPresenter::change_axis(QAbstractAxis* axis)
{
    QAbstractAxis* new_axis = nullptr;

    if (dynamic_cast<QValueAxis*>(axis))
        new_axis = new QLogValueAxis;
    else
        new_axis = new QValueAxis;

    new_axis->setTitleText(axis->titleText());
    chart()->addAxis(new_axis,axis->alignment());
    for (auto ser: chart()->series())
    {
        if (ser->attachedAxes().indexOf(axis) >= 0)
        {
            ser->detachAxis(axis);
            ser->attachAxis(new_axis);
        }
    }
    chart()->removeAxis(axis);
    delete axis;

    alter_menu();
}*/
