#include "admitanceringlayer.h"

#include "../viewmodel/plotitem.h"
#include "../viewmodel/functionitem.h"
#include <QPainter>

AdmitanceRingLayer::AdmitanceRingLayer(FunctionItem* func) :
    Layer(
        dynamic_cast<PlotItem*>(func->parent()),
        QString::fromStdString(func->value()->full_name()).append(" ").append(QObject::tr("koło admitancji")),
        func
        )
{

}

// distance from line described by normal @n and point @t to point @p
qreal AdmitanceRingLayer::distance(QVector2D n, QVector2D t, QVector2D p)
{
    qreal C = QVector2D::dotProduct(n,t);
    //qDebug() << "distance():: C=" << C;
    return QVector2D::dotProduct(n,p) + C;
}

void AdmitanceRingLayer::recalc()
{
    if (func_->top().y() - func_->bottom().y() > func_->right().x() - func_->left().x())
        middle_ = (func_->top() + func_->bottom()) / 2.0;
    else
        middle_ = (func_->right() + func_->left()) / 2.0;

    // finding f0 frequency
    QVector2D middle(middle_);
    QPointF closest;
    qreal dist = 1e20;

    QVector2D normal = QVector2D(-middle.y(), middle.x()).normalized();
    QVector2D z;
    //qreal origin_middle_dist = distance(middle.normalized(), middle, z);

    //qDebug() << "normal: " << normal << "  middle: " << middle << " origin-middle: " << origin_middle_dist;

    for (int i=0; i<func_->series()->count(); ++i){
        auto p = QVector2D(func_->series()->at(i));

        qreal curr_dist = std::abs(distance(normal,z,p));
        qreal curr_middle_dist = distance(middle.normalized(),middle,p);

        //qDebug() << "p: " << p << " curr_dist: " << curr_dist;

        //bool different_side = (curr_middle_dist * origin_middle_dist) <= 0;

        //qDebug() << "distance from LINE: " << curr_dist << "distance from NORMAL:" << curr_middle_dist;

        if (curr_dist < dist /*&& different_side*/){
            closest = func_->series()->at(i);
            dist = curr_dist;
        }
    }

    f0pos_ = closest;
    //qDebug() << "calced position: " << f0pos_;
}

void AdmitanceRingLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPointF start = plot_->chart()->mapToScene(0,0);
    QPointF end = plot_->chart()->mapToScene(f0pos_);

    painter->drawLine(start, end);
    //qDebug() << "painting from : to    " << start << " : " << end;

    QPointF sc_middle = plot_->chart()->mapToPosition(middle_);

    painter->setClipRect(plot_->chart()->plotArea());
    painter->setPen(QPen(Qt::gray, 1, Qt::DashLine));
    painter->drawLine(sc_middle+QPointF(0,200), sc_middle+QPointF(0,-200));
    painter->drawLine(sc_middle+QPointF(200,0), sc_middle+QPointF(-200,0));

    painter->setPen(QPen(Qt::gray, 2, Qt::DashDotLine));
    painter->drawLine(plot_->chart()->mapToPosition(QPointF(0,0)), plot_->chart()->mapToPosition(middle_*2.5));
}

void AdmitanceRingLayer::resize(QSize size)
{
    prepareGeometryChange();
    recalc();
}
