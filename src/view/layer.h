#ifndef VIEW_LAYER_H
#define VIEW_LAYER_H

#include <QGraphicsItem>
#include <vector>
#include <memory>

class PlotItem;
class FunctionItem;

/**
 * @brief The Layer class
 * @desc Stores additional graphic items which are displayed on the chart.
 */
class Layer : public QGraphicsItem
{
public:
    using ptr = std::unique_ptr<Layer>;

    Layer(PlotItem* plot, QString name, FunctionItem* func = nullptr);

    const QString& name() const;
    FunctionItem* func() const;
    PlotItem* plot() const;

    virtual void resize(QSize size);
    virtual void recalc();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

protected:
    PlotItem* plot_;
    FunctionItem* func_;
    QString name_;

};

/**
 * @brief The LayerStore class stores multiple layers and allows to manage them.
 * It is intended to be use in PlotItem instance.
 */
class LayerStack : public QGraphicsItem
{
public:
    LayerStack();

    void add_layer(Layer* layer);
    void remove_layer(Layer* layer);
    void remove_func_layers(FunctionItem* func);

    void resize(QSize size);
    void recalc();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:
    std::vector<std::unique_ptr<Layer>> layers_;
    QSize size_;
};

#endif // VIEW_LAYER_H
