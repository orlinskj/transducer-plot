#include "plotpresenter.h"

#include <algorithm>
#include <sstream>

#include <QGraphicsLayout>
#include <QStyleOptionGraphicsItem>
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
    broom_(new Broom),
    drag_enabled_(false)
{
    broom_->set_visibility(false);
    scene()->addItem(broom_);

    setRenderHint(QPainter::Antialiasing);
    setContentsMargins(0,0,0,0);
    //setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFocusPolicy(Qt::ClickFocus);

    setMouseTracking(true);

    // plot store signals
    QObject::connect(store_, &PlotStoreItemModel::plot_changed,
                     this, [this](PlotItem* p){ Q_UNUSED(p); alter_menu();});
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
        QString change_to(tr("liniowa"));
        if (dynamic_cast<QValueAxis*>(axis))
            change_to = tr("logarytmiczna");

        QString text(QString("\"")+axis->titleText() + "\" na: " + change_to);
        QAction* action = menu_.addAction(text);
        connect(action, &QAction::triggered,
                this, [axis,this](){
                        plot_->change_axis_type(axis);
                        alter_menu();
                        broom_->update_position();
                        });
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
        // broom
        auto value = chart()->mapToValue(chart()->mapFromScene(broom_->pos()));
        /*qDebug() << "value = " << value;
        qDebug() << "old pos = " << broom_->pos();*/
        // chart
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        chart()->resize(event->size());
        // broom continuation
        auto new_pos = chart()->mapToScene(chart()->mapToPosition(value));
        broom_->set_position(new_pos, true);
        //qDebug() << "new pos = " << new_pos;
    }
    QGraphicsView::resizeEvent(event);
    broom_->update_bounding_rect();
}

void PlotPresenter::mousePressEvent(QMouseEvent *event)
{
    if (chart())
    {
        drag_pos_ = event->pos();
        zoom_pos_ = event->pos();
        broom_click_pos_ = event->pos();

        if (event->buttons() & Qt::LeftButton)
        {
            if (event->modifiers() & Qt::ControlModifier)
            {
                zoom_enabled_ = true;
                zoom_button_ = Qt::LeftButton;
            }
            else if(event->modifiers() & Qt::ShiftModifier)
            {
                //zoom_enabled_ = true;
                //zoom_button_ = Qt::LeftButton;
            }
            else
            {
                drag_enabled_ = true;
                drag_button_ = Qt::LeftButton;
                broom_click_ = true;
            }
        }
    }
}

void PlotPresenter::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Left:
        chart()->scroll(2,0);
        break;
    case Qt::Key_Right:
        chart()->scroll(-2,0);
        break;
    case Qt::Key_Up:
        chart()->scroll(0,2);
        break;
    case Qt::Key_Down:
        chart()->scroll(0,-2);
        break;
    case Qt::Key_Plus:
        chart()->zoomIn();
        break;
    case Qt::Key_Minus:
        chart()->zoomOut();
        break;
    }

    broom_->update_position();
}

void PlotPresenter::mouseReleaseEvent(QMouseEvent *event)
{
    if (!(event->buttons() & drag_button_))
        drag_enabled_ = false;
    if (!(event->buttons() & zoom_button_))
        zoom_enabled_ = false;

    if (broom_click_ && !(event->buttons() & Qt::LeftButton))
    {
        broom_click_ = false;
        if (broom_click_pos_ == event->pos())
        {
            broom_->set_position(event->pos());
            broom_->toggle();
        }
    }
}

void PlotPresenter::mouseMoveEvent(QMouseEvent *event)
{
    if (chart())
    {
        // dragging
        if (drag_enabled_)
        {
            auto delta = event->pos() - drag_pos_;
            chart()->scroll(-delta.x(),delta.y());
            drag_pos_ = event->pos();
            broom_->update_position();
        }
        // zooming
        else if (zoom_enabled_)
        {
            double k = 100.0;
            auto delta = event->pos() - zoom_pos_;
            chart()->zoom(k/(k+delta.y()));
            zoom_pos_ = event->pos();
            broom_->update_position();
        }

        // broom
        if (chart()->plotArea().contains(event->pos()))
        {
            broom_->set_visibility(true);
            broom_->set_position(event->pos());
        }
        else
            broom_->set_visibility(false);
    }

    QGraphicsView::mouseMoveEvent(event);
}

void PlotPresenter::context_menu(const QPoint& point)
{
    if (menu_.isEnabled())
        menu_.exec(this->mapToGlobal(point));
}

QImage PlotPresenter::screenshot(int width, int height)
{
    QImage img(width,height,QImage::Format_ARGB32);
    img.fill(Qt::white);

    QPainter painter(&img);
    painter.setRenderHint(QPainter::Antialiasing);

    auto tmp_size = this->size();
    this->resize(width,height);
    scene()->render(&painter);
    this->resize(tmp_size);

    /*if (!img.save("/home/janek/ss.png","png",70))
        throw std::runtime_error("Cannot save to a file");*/

    return img;
}
