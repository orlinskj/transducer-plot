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

#include "../view/unitaxis.h"
#include "../viewmodel/plotstoreitemmodel.h"

PlotPresenter::PlotPresenter(PlotStoreItemModel *store) :
    QGraphicsView(new QGraphicsScene, nullptr),
    plot_(nullptr),
    store_(store),
    broom_(new Broom),
    drag_enabled_(false),
    zoom_enabled_(false),
    zoom_horizontal_(false)
{
    broom_->set_visibility(false);
    scene()->addItem(broom_);

    //setRenderHint(QPainter::Antialiasing);
    setContentsMargins(0,0,0,0);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFocusPolicy(Qt::ClickFocus);

    setMouseTracking(true);

    // plot store signals
    connect(store_, &PlotStoreItemModel::plot_changed,
            this, &PlotPresenter::plot_changed);

    connect(store_, &PlotStoreItemModel::plot_removed,
            this, &PlotPresenter::plot_removed);

    // setting up menu
    menu_.setEnabled(false);
    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
                     this, SLOT(context_menu(QPoint)));
}

PlotPresenter::~PlotPresenter() {}

void PlotPresenter::plot_changed(PlotItem* p){
    alter_menu();
    alter_axes();
}

void PlotPresenter::plot_removed(PlotItem* p){
    if (plot_ == p){
        scene()->removeItem(p->chart());
        plot_ = nullptr;
        broom_->set_plot(nullptr);
    }

    alter_menu();
}

void PlotPresenter::show_plot(PlotItem *plot)
{
    if (plot_ == plot)
        return;

    if (plot_){
        this->scene()->removeItem(&(plot_->layers()));
    }

    broom_->set_plot(plot);

    if (chart())
        this->scene()->removeItem(chart());

    plot_ = plot;

    if (plot_){
        scene()->addItem(plot_->chart());
        scene()->addItem(&(plot_->layers()));

        plot_->chart()->resize(this->size());
        plot_->chart()->setAcceptHoverEvents(true);

        plot_->layers().resize(size());
    }

    alter_menu();
    alter_axes();
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

PlotStoreItemModel *PlotPresenter::store()
{
    return store_;
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
        // chart
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        chart()->resize(event->size());
        // broom continuation
        auto new_pos = chart()->mapToScene(chart()->mapToPosition(value));
        broom_->set_position(new_pos, true);
    }
    QGraphicsView::resizeEvent(event);
    broom_->update_bounding_rect();
    if (plot_)
        plot_->layers().resize(event->size());
    alter_axes();
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
            if (event->modifiers() & Qt::ControlModifier || zoom_horizontal_)
            {
                zoom_enabled_ = true;
                broom_->setVisible(false);
            }

            else
            {
                drag_enabled_ = true;
                broom_click_ = true;
                broom_->setVisible(false);
            }
        }
    }
}

void PlotPresenter::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_X:
        zoom_horizontal_ = true;
        break;
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
    case Qt::Key_N:
        for (auto axis : chart()->axes(Qt::Vertical))
        {
            if (auto value_axis = dynamic_cast<QValueAxis*>(axis))
                value_axis->applyNiceNumbers();
        }
    }

    broom_->update_position();
}

void PlotPresenter::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_X:
        zoom_horizontal_ = false;
        break;
    }
}

void PlotPresenter::mouseReleaseEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
    {
        drag_enabled_ = false;
        broom_->setVisible(true);
    }
    if (!(event->buttons() & Qt::LeftButton))
    {
        zoom_enabled_ = false;
        broom_->setVisible(true);
    }

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
            plot_->layers().recalc();
        }
        // zooming
        else if (zoom_enabled_)
        {
            double k = 200.0;
            auto delta = QVector2D(event->pos() - zoom_pos_).y();
            double factor = k / (k+delta);

            if (zoom_horizontal_){
                // getting range
                auto value_axis = dynamic_cast<QValueAxis*>(chart()->axisX());

                if (value_axis){
                    auto range = (value_axis->max() - value_axis->min());
                    if (delta != 0){
                        range = range * factor;
                    }
                    qDebug() << "range: " << range << "delta:" << delta;
                    auto diff = range - (value_axis->max() - value_axis->min());
                    value_axis->setMin(value_axis->min()+diff/2.0);
                    value_axis->setMax(value_axis->max()-diff/2.0);
                }
                else{
                    qDebug() << "any type other than QValueAxis is not supported for X axis";
                }
            }
            else{
                chart()->zoom(factor);
            }

            alter_axes();
            zoom_pos_ = event->pos();
            broom_->update_position();
            plot_->layers().recalc();

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

void PlotPresenter::alter_axes()
{
    if (!chart() || !chart()->axisX())
        return;

    qreal xmin, xrange;

    auto x_axis = chart()->axisX();
    auto x_unit_axis = UnitAxis::from_qabstractaxis(x_axis);

    if (x_unit_axis->unit_axis_type() == UnitAxis::Type::Value){
        QValueAxis* value_axis = dynamic_cast<UnitValueAxis*>(x_unit_axis);
        //xmax = value_axis->max();
        xmin = value_axis->min();
    }
    else{
        QLogValueAxis* value_axis = dynamic_cast<UnitLogValueAxis*>(x_unit_axis);
        //xmax = value_axis->max();
        xmin = value_axis->min();
    }

    for(auto y_axis : chart()->axes(Qt::Vertical))
    {
        auto y_unit_axis = UnitAxis::from_qabstractaxis(y_axis);
        if (y_unit_axis->unit().match_unit(x_unit_axis->unit())){
            // getting Y range
            if (y_unit_axis->unit_axis_type() == UnitAxis::Type::Value){
                QValueAxis* value_axis = dynamic_cast<UnitValueAxis*>(y_unit_axis);
                xrange = (value_axis->max()-value_axis->min()) * chart()->plotArea().size().width() / chart()->plotArea().size().height();
            }
            else{
                QLogValueAxis* value_axis = dynamic_cast<UnitLogValueAxis*>(y_unit_axis);
                xrange = (value_axis->max()-value_axis->min()) * chart()->plotArea().size().width() / chart()->plotArea().size().height();
            }

            // setting calculated range
            if (x_unit_axis->unit_axis_type() == UnitAxis::Type::Value){
                QValueAxis* value_axis = dynamic_cast<UnitValueAxis*>(x_unit_axis);
                value_axis->setRange(xmin,xmin+xrange);
            }
            else{
                QLogValueAxis* value_axis = dynamic_cast<UnitLogValueAxis*>(x_unit_axis);
                value_axis->setRange(xmin,xmin+xrange);
            }

            return;
        }
    }
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

    painter.end();

    return img;
}
