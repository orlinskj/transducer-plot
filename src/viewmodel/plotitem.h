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

    QChart* chart() const;
    TreeItem* append(TreeItem* item) override; // from TreeItem

protected:
    QChart* chart_;

};


#endif // PLOTADAPTER_H
