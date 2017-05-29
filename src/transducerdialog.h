#ifndef TABLEDATADIALOG_H
#define TABLEDATADIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QGraphicsPixmapItem>
#include <memory>
#include <tuple>

#include "model/solver.h"

namespace Ui {
class TransducerDialog;
}

class TreeItemModel;
class TransducerItem;
class TransducerTableProxyModel;

class TransducerDialog : public QDialog
{
    Q_OBJECT
    using Label = std::tuple<std::string, QPointF, bool>;

public:
    explicit TransducerDialog(QWidget *parent = 0, TreeItemModel* transducer_model = nullptr);
    ~TransducerDialog();

    void transducer_changed(TransducerItem* t);
    void place_pixmap_labels(const std::vector<Label>& labels);

public slots:
    void export_transducer();
    void set_tab(int tab);
    void model_type_changed(int index);
    void export_type_changed(int index);
    void recalc_model();
    void fixed_capacity_checkbox(int state);

private:
    Ui::TransducerDialog *ui;
    std::unique_ptr<TransducerTableProxyModel> transducer_table_model_;
    TreeItemModel* transducer_model_;

    QStandardItemModel table_params_model_;
    QGraphicsPixmapItem* pixmap_;

    BVDSolver solver_;

    static std::vector<Label> labels_series;
    static std::vector<Label> labels_parallel;
};


#endif // TABLEDATADIALOG_H
