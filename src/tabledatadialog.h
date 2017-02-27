#ifndef TABLEDATADIALOG_H
#define TABLEDATADIALOG_H

#include <QDialog>
// #include "viewmodel/transducertableproxymodel.h"
#include <memory>

namespace Ui {
class TableDataDialog;
}

class TreeItemModel;
class TransducerItem;
class TransducerTableProxyModel;

class TableDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TableDataDialog(QWidget *parent = 0, TreeItemModel* transducer_model = nullptr);
    ~TableDataDialog();

    void set_transducer(TransducerItem* t);

private:
    Ui::TableDataDialog *ui;
    std::unique_ptr<TransducerTableProxyModel> transducer_table_model_;
    TreeItemModel* transducer_model_;
};


#endif // TABLEDATADIALOG_H
