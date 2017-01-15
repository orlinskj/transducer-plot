#ifndef MODEL_PLOTSTORE_H
#define MODEL_PLOTSTORE_H

//#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <vector>

#include "plot.h"

namespace ac{

class PlotStoreItemModel : public QAbstractItemModel
{
public:
    PlotStoreItemModel();

    Plot* add_plot();
    void remove_plot(Plot *plot);

    // new implementation based on QAbstractItemModel
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

protected:
    std::vector<Plot_ptr>::const_iterator get_plot_iter(const Plot *plot) const;

    std::vector<Plot_ptr> plots_;
};

}

#endif // PLOTSTORE_H
