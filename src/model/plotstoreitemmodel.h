#ifndef MODEL_PLOTSTORE_H
#define MODEL_PLOTSTORE_H

#include <QAbstractItemModel>
#include <vector>

#include "plot.h"
#include "treemodel/treeitemmodel.h"

namespace ac{

class PlotStoreItemModel : public TreeItemModel
{
Q_OBJECT

public:
    PlotStoreItemModel(QObject *parent = nullptr);
    virtual ~PlotStoreItemModel() override;

    Plot* add_plot(Plot *plot);
    void remove_plot(Plot *plot);

    Function* add_function(Plot* plot, Function* func);
    void remove_function(const Function* func);

    // new implementation based on QAbstractItemModel
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool hasChildren(const QModelIndex &parent) const override;

    QModelIndex index(Plot* plot) const;
    QModelIndex index(Function* func) const;

protected:
    int plot_row(const Plot* plot) const;

    QObject* parent_;
    std::vector<Plot*> plots_;
};

}

#endif // PLOTSTORE_H
