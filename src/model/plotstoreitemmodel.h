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
    ~PlotStoreItemModel();

    Plot* add_plot();
    void remove_plot(Plot *plot);

    // new implementation based on QAbstractItemModel
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

protected:
    std::vector<Plot*>::const_iterator get_plot_iter(const Plot *plot) const;

    std::vector<Plot*> plots_;
};

}

#endif // PLOTSTORE_H
