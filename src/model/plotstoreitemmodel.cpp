#include "plotstoreitemmodel.h"

#include <memory>
#include <QModelIndex>

ac::PlotStoreItemModel::PlotStoreItemModel(QObject* parent) :
    parent_(parent)
{

}

ac::PlotStoreItemModel::~PlotStoreItemModel()
{
    for (auto&& plot : plots_)
    {
        delete plot;
    }
}

ac::Plot* ac::PlotStoreItemModel::add_plot(ac::Plot* plot)
{
    beginInsertRows(QModelIndex(),rowCount(),rowCount());
    plots_.push_back(plot);
    endInsertRows();
    return *(plots_.rbegin());
}

int ac::PlotStoreItemModel::plot_row(const Plot *plot) const
{
    for (auto it=plots_.cbegin(); it!=plots_.cend(); it++)
    {
        if (*it == plot)
            return int(it-plots_.cbegin());
    }

    return -1;
}

void ac::PlotStoreItemModel::remove_plot(Plot* plot)
{
    for (auto it=plots_.cbegin(); it!=plots_.cend(); it++)
    {
        if (*it == plot)
        {
            int row = int(it-plots_.cbegin());
            beginRemoveRows(QModelIndex(), row, row);
            plots_.erase(it);
            endRemoveRows();
            break;
        }
    }
}

ac::Function* ac::PlotStoreItemModel::add_function(Plot* plot, Function* func)
{
    beginInsertRows(index(plot), plot->function_count(), plot->function_count());
    plot->add_function(func);
    endInsertRows();
    return func;
}


// QAbstractModelImplementation

QModelIndex ac::PlotStoreItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent == QModelIndex())
    {
        if (row >= 0 && row < int(plots_.size()) && !column )
            return this->createIndex(row, 0, (void*)plots_[row]);
        else
            return QModelIndex();
    }
    else
    {
        auto plot = static_cast<Plot*>(parent.internalPointer());
        if (row >= 0 && row < plot->function_count())
        {
            return this->createIndex(row, 0, (void*)plot->function_at(row));
        }
        else
        {
            return QModelIndex();
        }
    }
}

bool ac::PlotStoreItemModel::hasChildren(const QModelIndex &parent) const
{
    if (parent == QModelIndex())
    {
        return bool(plots_.size());
    }
    else
    {
        auto item_ptr = static_cast<ac::PlotStoreItem*>(parent.internalPointer());

        auto plot = dynamic_cast<ac::Plot*>(item_ptr);
        if (plot)
            return bool(plot->function_count());

        return false;
    }
}

QModelIndex ac::PlotStoreItemModel::parent(const QModelIndex &child) const
{
    auto item_ptr = static_cast<ac::PlotStoreItem*>(child.internalPointer());

    if (dynamic_cast<Plot*>(item_ptr))
    {
        return QModelIndex();
    }
    else if (auto func_ptr = dynamic_cast<Function*>(item_ptr))
    {
        int row = plot_row(func_ptr->get_plot());
        return createIndex(row,0,(void*)func_ptr);
    }
    else
    {
        return QModelIndex();
    }
}

int ac::PlotStoreItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent == QModelIndex())
        return plots_.size();
    else
    {
        auto plot = dynamic_cast<ac::Plot*>((ac::PlotStoreItem*)parent.internalPointer());
        if (plot)
            return plot->function_count();

        return 0;
    }
}

int ac::PlotStoreItemModel::columnCount(const QModelIndex &parent) const
{
    if (parent == QModelIndex())
        return plots_.size() ? 1 : 0;
    else
    {
        auto plot = dynamic_cast<ac::Plot*>((ac::PlotStoreItem*)parent.internalPointer());
        if (plot)
            return plot->function_count() ? 1 : 0;

        return 0;
    }
}

QVariant ac::PlotStoreItemModel::data(const QModelIndex &index, int role) const
{
    auto item_ptr = static_cast<ac::PlotStoreItem*>(index.internalPointer());

    if (role == Qt::DisplayRole)
    {
        return QVariant::fromValue(QString(item_ptr->to_string().c_str()));
    }
    else if (role == ac::PlotStoreItem::Role)
    {
        auto plot = dynamic_cast<ac::Plot*>(item_ptr);
        if (plot)
            return QVariant::fromValue(plot);

        auto func = dynamic_cast<ac::Function*>(item_ptr);
        if(func)
            return QVariant::fromValue(func);

        return QVariant();
    }
    else
        return QVariant();

}

QModelIndex ac::PlotStoreItemModel::index(Plot *plot) const
{
    return createIndex(plot_row(plot),0,(void*)plot);
}

QModelIndex ac::PlotStoreItemModel::index(Function* func) const
{
    return createIndex(func->get_plot()->function_row(func),0,(void*)func);
}
