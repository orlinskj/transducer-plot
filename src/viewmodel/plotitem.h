#ifndef PLOTADAPTER_H
#define PLOTADAPTER_H

#include <QChart>

#include "../model/plot.h"
#include "../model/function.h"
#include "../viewmodel/treemodel/treeitemt.h"

using namespace QtCharts;


class FunctionItem;


class PlotItem : public TreeItemTOwner<Plot>
{
public:
    PlotItem();
    PlotItem(Plot* plot);
    ~PlotItem();

    QChart* chart() const;
    void change_axis_type(QAbstractAxis* axis);

    TreeItem* append(TreeItem* item) override; // from TreeItem
    void remove(TreeItem* item) override; // from TreeItem

//signals:
    //void function_added(FunctionItem* f);
    //void function_removed(FunctionItem* f);

protected:
    QList<QAbstractSeries*> axis_series(QAbstractAxis* axis);
    void change_axis_alignment(QAbstractAxis* axis, Qt::Alignment align);

    QChart* chart_;

};


#endif // PLOTADAPTER_H
