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

#include "../viewmodel/plotstoreitemmodel.h"

PlotPresenter::PlotPresenter(PlotStoreItemModel *store) :
    QGraphicsView(new QGraphicsScene, nullptr),
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

    // plot store signals
    QObject::connect(store_, &PlotStoreItemModel::plot_changed,
                     this,
                     [this](PlotItem* p){
        broom_->set_plot(p);
        alter_menu();
    });
    QObject::connect(store_, &PlotStoreItemModel::plot_removed,
                     this,
                     [this](PlotItem* p)
    {
        broom_->set_plot(nullptr);
        if (p->chart() == this->chart())
            scene()->removeItem(p->chart());
        plot_ = nullptr;
        alter_menu();
    });

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
    if (!chart())
    {
        menu_.setEnabled(false);
        return;
    }

    menu_.setEnabled(true);
    menu_.clear();

    for (auto axis: chart()->axes(Qt::Vertical))
    {
        QString change_to("liniowa");
        if (dynamic_cast<QValueAxis*>(axis))
            change_to = "logarytmiczna";

        QString text(QString("\"")+axis->titleText() + "\" na: " + change_to);
        QAction* action = menu_.addAction(text);
        ///TODO add action here
        connect(action, &QAction::triggered,
                this, [axis,this](){ plot_->change_axis_type(axis); alter_menu(); });
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
    if (chart())
    {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        chart()->resize(event->size());
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
    if (chart())
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
