#include "transducerdialog.h"
#include "ui_transducerdialog.h"
#include "viewmodel/treemodel/treeitemmodel.h"
#include "viewmodel/transducertableproxymodel.h"
#include "viewmodel/setitem.h"
#include "view/transducerdelegate.h"

#include <QComboBox>
#include <QFileDialog>
#include "io/tablemodelfilehandler.h"
#include <string>
#include <boost/filesystem.hpp>

TransducerDialog::TransducerDialog(QWidget *parent, TreeItemModel* transducer_model) :
    QDialog(parent),
    ui(new Ui::TransducerDialog),
    transducer_table_model_(nullptr),
    transducer_model_(transducer_model)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);
    // getting first transducer
    if (transducer_model_->rowCount(QModelIndex()))
    {
        auto transducer = dynamic_cast<TransducerItem*>(transducer_model_->child(0));
        set_transducer(transducer);
    }

    ui->tableView->verticalHeader()->setDefaultSectionSize(18);

    ui->transducerComboBox->setModel(transducer_model);
    connect(ui->transducerComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                     this,
                     [this](int index){
                        set_transducer(dynamic_cast<TransducerItem*>(transducer_model_->child(index)));
                     });

    connect(ui->saveButton, &QPushButton::clicked,
            this, &TransducerDialog::export_as);


    QList<QStandardItem*> items = {
        new QStandardItem(tr("Szeregowy")),
        new QStandardItem(tr("Równoległy"))
    };
    type_model_.appendColumn(items);
    ui->transducerComboBox->setItemDelegate(new TransducerDelegate);
    ui->comboBoxModelType->setModel(&type_model_);
}

TransducerDialog::~TransducerDialog()
{ }

void TransducerDialog::export_as()
{
    ///TODO add handling of empty combobox
    auto transducer_item = dynamic_cast<TransducerItem*>(
                transducer_model_->child(ui->transducerComboBox->currentIndex()));
    QString save_file_name = QFileDialog::getSaveFileName(
                this,"Eksport",QString::fromStdString(transducer_item->value()->get_name()),
                "plik CSV (*.csv)");

    if (save_file_name.isEmpty())
        return;

    boost::filesystem::path path(save_file_name.toStdString());

    if (path.extension().empty())
        path.replace_extension(".csv");


    TableModelFileHandler file_handler(transducer_table_model_.get());
    std::string x = path.string();
    file_handler.save(path.string());
}

void TransducerDialog::set_tab(int tab)
{
    //if (tab >= 0 && tab < ui->tabWidget->children().size())
    ui->tabWidget->setCurrentIndex(tab);
}

void TransducerDialog::set_transducer(TransducerItem* t)
{
    ui->tableView->setModel(nullptr);
    transducer_table_model_.reset(new TransducerTableProxyModel(t));
    ui->tableView->setModel(transducer_table_model_.get());

}
