#include "tabledatadialog.h"
#include "ui_tabledatadialog.h"
#include "viewmodel/treemodel/treeitemmodel.h"
#include "viewmodel/transducertableproxymodel.h"
#include "viewmodel/setitem.h"

#include <QComboBox>

TableDataDialog::TableDataDialog(QWidget *parent, TreeItemModel* transducer_model) :
    QDialog(parent),
    ui(new Ui::TableDataDialog),
    transducer_table_model_(nullptr),
    transducer_model_(transducer_model)
{
    ui->setupUi(this);
    // getting first transducer
    if (transducer_model_->rowCount(QModelIndex()))
    {
        auto transducer = dynamic_cast<TransducerItem*>(transducer_model_->child(0));
        set_transducer(transducer);
    }

    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);

    ui->transducerComboBox->setModel(transducer_model);
    connect(ui->transducerComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                     this,
                     [this](int index){
                        set_transducer(dynamic_cast<TransducerItem*>(transducer_model_->child(index)));
                     });
}

TableDataDialog::~TableDataDialog()
{ }

void TableDataDialog::set_transducer(TransducerItem* t)
{
    ui->tableView->setModel(nullptr);
    transducer_table_model_.reset(new TransducerTableProxyModel(t));
    ui->tableView->setModel(transducer_table_model_.get());

    /*auto sets = t->children();

    int i=0;
    for ( auto set : sets)
    {
        qDebug() << i;
        auto casted = dynamic_cast<SetItem*>(set);
        transducer_table_model_->setHeaderData(
                    i,
                    Qt::Horizontal,
                    QString::fromStdString(casted->value()->unit().name()),
                    Qt::DisplayRole);
        i++;
    }*/
}
