#ifndef MODEL_PLOTSTORE_H
#define MODEL_PLOTSTORE_H

#include <QAbstractItemModel>
#include <vector>

#include "../viewmodel/plotitem.h"
#include "treemodel/treeitemmodel.h"

/**
 * @brief The PlotStoreItemModel class
 * @desc Model storing PlotItem and FunctionItem objects. Emits signals on every plot update.
 */
class PlotStoreItemModel : public TreeItemModel
{
Q_OBJECT

public:
    PlotStoreItemModel(QObject *parent = nullptr);
    virtual ~PlotStoreItemModel();

    void emit_begin_insert_rows(int first, int last, TreeItem* parent) override;
    void emit_end_insert_rows(int first, int last, TreeItem* parent) override;
    void emit_begin_remove_rows(int first, int last, TreeItem* parent) override;
    void emit_end_remove_rows(int first, int last, TreeItem* parent) override;

signals:
    void plot_changed(PlotItem* plot);
    void plot_removed(PlotItem* plot);
    void plot_added(PlotItem* plot);

protected:
    PlotItem* removed_;

};


#endif // PLOTSTORE_H
