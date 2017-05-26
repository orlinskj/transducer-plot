#include "layer.h"

#include "../viewmodel/functionitem.h"
#include "../viewmodel/plotitem.h"

Layer::Layer(PlotItem *plot, QString name, FunctionItem *func) :
    plot_(plot),
    func_(func),
    name_(name)
{

}

const QString& Layer::name() const
{
    return name_;
}

FunctionItem* Layer::func() const
{
    return func_;
}

PlotItem* Layer::plot() const
{
    return plot_;
}

void Layer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // should be subclassed...
}

QRectF Layer::boundingRect() const
{
    return QRectF(QPointF(0,0), plot_->chart()->size());
}

void Layer::resize(QSize size)
{
    prepareGeometryChange();
}

void Layer::recalc()
{

}

/*** -------------- LayerStore ------------------ ***/

LayerStack::LayerStack()
{
    this->setZValue(900);
}

void LayerStack::add_layer(Layer *layer)
{
    layers_.emplace_back(Layer::ptr(layer));
    layer->resize(size_);
}

void LayerStack::remove_layer(Layer *layer)
{
    std::remove_if(layers_.begin(), layers_.end(), [layer](const Layer::ptr& ptr){
        return ptr.get() == layer;
    });

}

void LayerStack::remove_func_layers(FunctionItem *func)
{
    for(auto it=layers_.begin(); it!=layers_.end(); ){
        if ((*it)->func() == func){
            layers_.erase(it);
        }
        else{
            ++it;
        }
    }
}

void LayerStack::resize(QSize size)
{
    for (auto& ptr : layers_){
        ptr->resize(size);
    }
    size_ = size;
}

void LayerStack::recalc()
{
    for (auto& ptr : layers_){
        ptr->recalc();
    }
}

void LayerStack::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    for (auto& ptr : layers_){
        ptr->paint(painter, option, widget);
    }
}

QRectF LayerStack::boundingRect() const
{
    return QRectF(QPointF(0,0), size_);
}
