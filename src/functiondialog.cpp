#include "functiondialog.h"
#include "ui_functiondialog.h"

#include "view/transducerdelegate.h"
#include "viewmodel/setitem.h"
#include "viewmodel/transduceritem.h"
#include "viewmodel/functionitem.h"
#include "view/plotpresenter.h"

FunctionDialog::FunctionDialog(QWidget *parent,
                               TreeItemModel *model,
                               PlotItem *plot) :
    QDialog(parent),
    ui(new Ui::FunctionDialog)
{
    ui->setupUi(this);
    ui->transducerView->setModel(model);
    ui->transducerView->setItemDelegate(new TransducerDelegate);

    plot_ = plot;

    domain_model_.setSourceModel(model);
    //ui->domainView->setModel(&domain_model_);
    //ui->codomainView->setModel(&domain_model_);

    ui->domainView->setModel(&sets_model_);
    ui->codomainView->setModel(&sets_model_);

    QObject::connect(ui->transducerView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                     this,SLOT(transducer_selection_changed(const QItemSelection&)));
}

void FunctionDialog::transducer_selection_changed(const QItemSelection &selection)
{
    sets_model_.kill_children();
    if (!selection.indexes().isEmpty())
    {
        QModelIndex transducer_index = selection.indexes().first();
        auto item = static_cast<TreeItem*>(transducer_index.internalPointer());
        auto t_adapter =
                dynamic_cast<TransducerItem*>(item);
        for(int i=0; i<t_adapter->children_count(); i++)
        {
            auto set = dynamic_cast<SetItem*>(t_adapter->child(i));
            sets_model_.append(new SetItem((*set)()));
        }
        //domain_model_.set_branch(static_cast<TreeItem*>(transducer_index.internalPointer()));

    }
}

FunctionDialog::~FunctionDialog()
{
    delete ui;
}

void FunctionDialog::on_buttonBox_accepted()
{
    QModelIndex t_item, d_item, c_item;
    if (ui->transducerView->selectionModel()->hasSelection())
        t_item = ui->transducerView->selectionModel()->selectedIndexes().first();
    if (ui->domainView->selectionModel()->hasSelection())
        d_item = ui->domainView->selectionModel()->selectedIndexes().first();
    if (ui->codomainView->selectionModel()->hasSelection())
        c_item = ui->codomainView->selectionModel()->selectedIndexes().first();

    auto transducer = dynamic_cast<TransducerItem*>(static_cast<TreeItem*>(t_item.internalPointer()));
    auto domain = dynamic_cast<SetItem*>(static_cast<TreeItem*>(d_item.internalPointer()));
    auto codomain = dynamic_cast<SetItem*>(static_cast<TreeItem*>(c_item.internalPointer()));

    if (transducer && domain && codomain)
    {
        auto func = new Function((*transducer)(),(*domain)(),(*codomain)());
        plot_->append(new FunctionItem(func));
    }
}
