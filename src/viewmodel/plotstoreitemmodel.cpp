#include "plotstoreitemmodel.h"

#include <memory>
#include <QModelIndex>

PlotStoreItemModel::PlotStoreItemModel(QObject* parent) :
    TreeItemModel(parent)
{
    plot_to_be_changed_ = nullptr;
    plot_to_be_removed_ = nullptr;
    plot_to_be_added_ = false;
}

PlotStoreItemModel::~PlotStoreItemModel() { }

void PlotStoreItemModel::emit_begin_insert_rows(int first, int last, std::vector<int>* tree)
{
    TreeItemModel::emit_begin_insert_rows(first,last,tree);

    if (tree && tree->size())
    {
        TreeItem* item = nullptr;
        while (tree->size())
        {
            item = item ? item->child(tree->back()) : child(tree->back());
            if (auto plot = dynamic_cast<PlotItem*>(item))
            {
                plot_to_be_changed_ = plot;
                break;
            }
            tree->pop_back();
        }
    }
    else
    {
        plot_to_be_added_ = true;
    }
}

void PlotStoreItemModel::emit_end_insert_rows()
{
    TreeItemModel::emit_end_insert_rows();
    if (plot_to_be_changed_)
    {
        emit plot_changed(plot_to_be_changed_);
        plot_to_be_changed_ = nullptr;
    }
    if (plot_to_be_added_)
    {
        emit plot_changed(dynamic_cast<PlotItem*>(child(children_count()-1)));
        plot_to_be_added_ = false;
    }
}

void PlotStoreItemModel::emit_begin_remove_rows(int first, int last, std::vector<int>* tree)
{
    TreeItemModel::emit_begin_remove_rows(first,last,tree);

    // this assumes that plots can only be first level nodes of tree
    if (!tree || !tree->size())
        plot_to_be_removed_ = dynamic_cast<PlotItem*>(child(first));
    else
    {
        TreeItem* item = nullptr;
        for (;!tree->empty();tree->pop_back())
        {
            item = child(tree->back());
            if (auto plot = dynamic_cast<PlotItem*>(item))
            {
                plot_to_be_changed_ = plot;
                break;
            }
        }
    }
}

void PlotStoreItemModel::emit_end_remove_rows()
{
    TreeItemModel::emit_end_remove_rows();
    if (plot_to_be_removed_)
    {
        emit plot_removed(plot_to_be_removed_);
        plot_to_be_removed_ = nullptr;
    }
    if (plot_to_be_changed_)
    {
        emit plot_changed(plot_to_be_changed_);
        plot_to_be_changed_ = nullptr;
    }
}
