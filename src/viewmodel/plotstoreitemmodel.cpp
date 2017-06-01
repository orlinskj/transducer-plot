#include "plotstoreitemmodel.h"

#include <memory>
#include <QModelIndex>

PlotStoreItemModel::PlotStoreItemModel(QObject* parent) : TreeItemModel(parent){ }

PlotStoreItemModel::~PlotStoreItemModel() { }

void PlotStoreItemModel::emit_begin_insert_rows(int first, int last, TreeItem* parent)
{
    TreeItemModel::emit_begin_insert_rows(first,last,parent);
}

void PlotStoreItemModel::emit_end_insert_rows(int first, int last, TreeItem* parent)
{
    TreeItemModel::emit_end_insert_rows(first,last,parent);

    auto plot_item = dynamic_cast<PlotItem*>(parent);

    if (plot_item){
        emit plot_changed(plot_item);
    }
    else if(parent){
        emit plot_added(dynamic_cast<PlotItem*>(parent->child(first)));
    }
}

void PlotStoreItemModel::emit_begin_remove_rows(int first, int last, TreeItem* parent)
{
    TreeItemModel::emit_begin_remove_rows(first,last,parent);
    removed_ = dynamic_cast<PlotItem*>(parent->child(first));
}

void PlotStoreItemModel::emit_end_remove_rows(int first, int last, TreeItem* parent)
{
    TreeItemModel::emit_end_remove_rows(first,last,parent);

    auto plot_item = dynamic_cast<PlotItem*>(parent);

    if (plot_item){
        emit plot_changed(plot_item);
    }
    else if(parent && parent->is_root()){
        emit plot_removed(removed_);
    }
}
