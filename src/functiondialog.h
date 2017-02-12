#ifndef FUNCTIONDIALOG_H
#define FUNCTIONDIALOG_H

#include <QDialog>
#include <QItemSelection>
#include "model/treemodel/treeitemmodel.h"
#include "model/treemodel/flattentreeproxymodel.h"
#include "model/treemodel/branchfilterproxymodel.h"
#include "model/plot.h"

namespace Ui {
class FunctionDialog;
}

class FunctionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FunctionDialog(QWidget *parent = 0, TreeItemModel* model = nullptr, Plot* plot = nullptr);
    ~FunctionDialog();

protected slots:
    void transducer_selection_changed(const QItemSelection &selection);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::FunctionDialog *ui;

    //FlattenTreeProxyModel domain_model_;
    BranchFilterProxyModel domain_model_;
    TreeItemModel sets_model_;

    Plot* plot_;
};

#endif // FUNCTIONDIALOG_H
