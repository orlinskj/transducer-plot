#ifndef PLOTADAPTER_H
#define PLOTADAPTER_H

#include <QChart>

#include "../model/plot.h"
#include "../model/function.h"
#include "../viewmodel/treemodel/treeitemt.h"
#include "../view/admitanceringlayer.h"
#include "../view/layer.h"

using namespace QtCharts;


class FunctionItem;


class PlotItem : public TreeItemTOwner<Plot>
{
public:
    static bool isFuncAdmitanceRing(FunctionItem* func);

    PlotItem();
    /*PlotItem(Plot* plot);*/
    ~PlotItem();

    QChart* chart() const;
    LayerStack& layers();
    void change_axis_type(QAbstractAxis* axis);

    TreeItem* append(TreeItem* item) override; // from TreeItem
    void remove(TreeItem* item) override; // from TreeItem

    void resize(QSize size);

protected:
    QList<QAbstractSeries*> axis_series(QAbstractAxis* axis);
    void change_axis_alignment(QAbstractAxis* axis, Qt::Alignment align);

    QChart* chart_;
    LayerStack layers_;
};


#endif // PLOTADAPTER_H
