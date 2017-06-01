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

/**
 * @brief The PlotItem class
 * @desc Adapter around Plot class which stores all needed data for displaying plot in the GUI.
 * It stores QtCharts::QChart and LayerStack internally. Intended for use in TreeItemModel.
 */
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

    static PlotItem* from_qmodelindex(const QModelIndex& index);

protected:
    QList<QAbstractSeries*> axis_series(QAbstractAxis* axis);
    void change_axis_alignment(QAbstractAxis* axis, Qt::Alignment align);

    QChart* chart_;
    LayerStack layers_;
};


#endif // PLOTADAPTER_H
