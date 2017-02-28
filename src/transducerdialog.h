#ifndef TABLEDATADIALOG_H
#define TABLEDATADIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <memory>

namespace Ui {
class TransducerDialog;
}

class TreeItemModel;
class TransducerItem;
class TransducerTableProxyModel;

class TransducerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransducerDialog(QWidget *parent = 0, TreeItemModel* transducer_model = nullptr);
    ~TransducerDialog();

    void set_transducer(TransducerItem* t);

public slots:
    void export_as();
    void set_tab(int tab);

private:
    Ui::TransducerDialog *ui;
    std::unique_ptr<TransducerTableProxyModel> transducer_table_model_;
    TreeItemModel* transducer_model_;
    QStandardItemModel type_model_;
};


#endif // TABLEDATADIALOG_H
