#ifndef MODEL_PLOTSTORE_H
#define MODEL_PLOTSTORE_H

#include <QAbstractItemModel>
#include <vector>

#include "../model/plot.h"
#include "treemodel/treeitemmodel.h"

// namespace ac{

class PlotStoreItemModel : public TreeItemModel
{
Q_OBJECT

public:
    PlotStoreItemModel(QObject *parent = nullptr);
    virtual ~PlotStoreItemModel() override;

    void emit_begin_insert_rows(int first, int last, std::vector<int>* tree);
    void emit_end_insert_rows();
    void emit_begin_remove_rows(int first, int last, std::vector<int>* tree);
    void emit_end_remove_rows();

signals:
    void plot_changed(Plot* plot);
    void plot_removed(Plot* plot);

protected:
    Plot* plot_to_be_changed_;
    Plot* plot_to_be_removed_;
    bool  plot_to_be_added_;

};

// } // namespace ac

#endif // PLOTSTORE_H
