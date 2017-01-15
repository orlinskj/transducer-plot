#include "plotstoreitemmodel.h"

#include <memory>
#include <QModelIndex>

ac::PlotStoreItemModel::PlotStoreItemModel()
{

}

ac::Plot* ac::PlotStoreItemModel::add_plot()
{
    layoutAboutToBeChanged();
    plots_.push_back(ac::Plot::make_ptr(Plot("Wykres")));
    return plots_.rbegin()->get();
    layoutChanged();
}

std::vector<Plot_ptr>::const_iterator ac::PlotStoreItemModel::get_plot_iter(const Plot* plot) const
{
    for(auto it=plots_.cbegin(); it != plots_.cend(); it++)
    {
        if (it->get() == plot)
            return it;
    }

    return plots_.cend();
}

void ac::PlotStoreItemModel::remove_plot(Plot* plot)
{
    auto it = get_plot_iter(plot);

    if (it != plots_.cend())
    {
        plots_.erase(it);
    }
}


// QAbstractModelImplementation

QModelIndex ac::PlotStoreItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent == QModelIndex())
    {
        if (row >= 0 && row < int(plots_.size()) )
        {
            /// TODO check this
            return this->createIndex(row, 0, (void*)&(plots_[row]) );
        }
        else{
            return QModelIndex();
        }
    }
    else
    {
        auto plot = static_cast<Plot*>(parent.internalPointer());
        if (row >= 0 && row < plot->function_count())
        {
            return this->createIndex(row, 0, (void*)&(plot->function_at(row)));
        }
        else
        {
            return QModelIndex();
        }
    }
}

QModelIndex ac::PlotStoreItemModel::parent(const QModelIndex &child) const
{
    auto item_ptr = static_cast<ac::PlotStoreItem*>(child.internalPointer());

    if (dynamic_cast<Plot_ptr*>(item_ptr))
    {
        return QModelIndex();
    }
    else if (auto func_ptr = dynamic_cast<Function*>(item_ptr))
    {
        auto it = get_plot_iter(func_ptr->get_plot());
        int row = it-plots_.cbegin();
        return createIndex(row,0,(void*)&(*it));
    }
    else
    {
        return QModelIndex();
    }
}

int ac::PlotStoreItemModel::rowCount(const QModelIndex &parent) const
{

    return plots_.size();
}

int ac::PlotStoreItemModel::columnCount(const QModelIndex &parent) const
{
    return plots_.size() ? 1 : 0;
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
        auto plot = dynamic_cast<ac::Plot_ptr*>(item_ptr);
        if (plot)
            return QVariant::fromValue(*plot);

        auto func = dynamic_cast<ac::Function*>(item_ptr);
        if(func)
            return QVariant::fromValue(*func);

        return QVariant();
    }
    else
        return QVariant();

}
