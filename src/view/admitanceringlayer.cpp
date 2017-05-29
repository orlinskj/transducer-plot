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
    return QVector2D::dotProduct(n,p) - C;
}

void AdmitanceRingLayer::recalc()
{
    if (func_->top().y() - func_->bottom().y() > func_->right().x() - func_->left().x())
        middle_ = (func_->top() + func_->bottom()) / 2.0;
    else
        middle_ = (func_->right() + func_->left()) / 2.0;

    // finding f0 frequency
    QVector2D middle(middle_);
    QPointF closest_pos;
    QPointF closest_neg;
    qreal dist_pos = 1e20;
    qreal dist_neg = -1e20;

    QVector2D normal = QVector2D(-middle.y(), middle.x()).normalized();
    qreal origin_middle_dist = distance(middle.normalized(), middle, QVector2D(0,0));

    for (int i=0; i<func_->series()->count(); ++i){
        auto p = QVector2D(func_->series()->at(i));

        qreal curr_dist = distance(normal,middle,p);
        qreal curr_middle_dist = distance(middle.normalized(),middle,p);

        bool different_side = (curr_middle_dist * origin_middle_dist) <= 0;

        if (different_side){
            if (curr_dist >= 0.0 && curr_dist < dist_pos){
                closest_pos = func_->series()->at(i);
                dist_pos = curr_dist;
            }
            if (curr_dist <= 0.0 && curr_dist > dist_neg){
                closest_neg = func_->series()->at(i);
                dist_neg = curr_dist;
            }
        }
    }

    if (dist_pos < 1e20 && dist_neg > -1e20){
        QVector2D a(closest_pos);
        QVector2D b(closest_neg);
        QVector2D dir = (b-a).normalized();
        f0pos_ = (a + (dir * dist_pos)).toPointF();

        // calculating frequency
        auto f_set = func_->value()->transducer()->get_set(Unit::Frequency);
        auto re_set = func_->value()->transducer()->get_set(Unit::ImpedanceReal);
        auto vals = Function(func_->value()->transducer(), re_set, f_set).values_at(f0pos_.x());

        qreal diff = 1e20;
        for (auto it=vals.cbegin(); it!=vals.cend(); ++it){
            auto curr_diff = std::abs(*it - f0pos_.x());
            if (curr_diff < diff){
                diff = curr_diff;
                f0_ = *it;
            }
        }
    }
    else{
        f0pos_ = QPointF();
    }


}

void AdmitanceRingLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPointF sc_middle = plot_->chart()->mapToPosition(middle_);
    qreal llen = 20;

    painter->setClipRect(plot_->chart()->plotArea());
    painter->setPen(QPen(Qt::gray, 1, Qt::DashLine));
    painter->drawLine(sc_middle+QPointF(0,llen), sc_middle+QPointF(0,-llen));
    painter->drawLine(sc_middle+QPointF(llen,0), sc_middle+QPointF(-llen,0));

    painter->setPen(QPen(Qt::magenta, 1, Qt::DashDotLine));
    painter->drawLine(plot_->chart()->mapToPosition(QPointF(0,0)), plot_->chart()->mapToPosition(middle_*2.5));

    painter->setPen(QPen(Qt::magenta, 1, Qt::SolidLine));
    painter->drawEllipse(plot_->chart()->mapToPosition(f0pos_),3,3);

    // frequency box
    if (QPointF() != f0pos_){
        painter->setRenderHint(QPainter::Antialiasing);
        QPointF box_pos = plot_->chart()->mapToPosition(f0pos_) + QPointF(7,-30);
        QSize box_size = QSize(115,5+3+15);
        QPainterPath path;
        path.addRoundedRect(QRectF(box_pos,box_size),3,3);
        QColor magneta = Qt::magenta;
        magneta.setAlpha(60);
        painter->fillPath(path,magneta);

        QFont font("Monospace",10);
        painter->setPen(Qt::black);

        painter->drawText(box_pos+QPointF(5,5+10), QString("f₀ ="));
        painter->setFont(font);
        painter->drawText(box_pos+QPointF(5+25,5+10), QString::number(f0_));
    }

}

void AdmitanceRingLayer::resize(QSize size)
{
    prepareGeometryChange();
    recalc();
}
