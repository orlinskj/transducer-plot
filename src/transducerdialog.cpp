#include "transducerdialog.h"
#include "ui_transducerdialog.h"
#include "viewmodel/treemodel/treeitemmodel.h"
#include "viewmodel/transducertableproxymodel.h"
#include "viewmodel/setitem.h"
#include "view/transducerdelegate.h"

#include <QComboBox>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include "io/tablemodelfilehandler.h"
#include <string>
#include <boost/filesystem.hpp>

std::vector<TransducerDialog::Label> TransducerDialog::labels_series = {
    TransducerDialog::Label("R",QPointF(245,220),true),
    TransducerDialog::Label("L",QPointF(245,90),true),
    TransducerDialog::Label("C",QPointF(330,155),false),
    TransducerDialog::Label("Co",QPointF(160,155),false)
};

std::vector<TransducerDialog::Label> TransducerDialog::labels_parallel = {
    TransducerDialog::Label("R",QPointF(170,155),false),
    TransducerDialog::Label("L",QPointF(250,155),false),
    TransducerDialog::Label("C",QPointF(330,155),false),
    TransducerDialog::Label("Co",QPointF(105,220),true)
};

TransducerDialog::TransducerDialog(QWidget *parent, TreeItemModel* transducer_model) :
    QDialog(parent),
    ui(new Ui::TransducerDialog),
    transducer_table_model_(nullptr),
    transducer_model_(transducer_model),
    pixmap_(nullptr)
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

    ui->graphicsViewModel->setScene(new QGraphicsScene);
    ui->graphicsViewModel->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsViewModel->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(ui->saveButton, &QPushButton::clicked,
            this, &TransducerDialog::export_as);

    connect(ui->comboBoxModelType, &QComboBox::currentTextChanged,
            this, &TransducerDialog::select_model);

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

void TransducerDialog::select_model(const QString& option)
{
    ui->graphicsViewModel->scene()->clear();
    pixmap_ = nullptr;
    if (option == tr("Szeregowy"))
    {
        pixmap_ = new QGraphicsPixmapItem(QPixmap(":/icons/bvd-series.svg"));
        ui->graphicsViewModel->scene()->addItem(pixmap_);
        place_pixmap_labels(labels_series);
    }
    else if (option == tr("Równoległy"))
    {
        pixmap_ = new QGraphicsPixmapItem(QPixmap(":/icons/bvd-parallel.svg"));
        ui->graphicsViewModel->scene()->addItem(pixmap_);
        place_pixmap_labels(labels_parallel);
    }
    pixmap_->setPos(0,0);
}

void TransducerDialog::place_pixmap_labels(const std::vector<TransducerDialog::Label>& labels)
{
    for (const auto& l : labels)
    {
        auto text = new QGraphicsTextItem(std::get<0>(l).c_str());
        ui->graphicsViewModel->scene()->addItem(text);
        QPointF pos = std::get<1>(l);
        pos.ry() = pixmap_->pixmap().size().height()-pos.y();
        text->setPos(pos);
        char first_letter = std::get<0>(l).at(0);

        if (!std::get<2>(l))
        {
            text->setRotation(-90);
            text->moveBy(-45,15);
            if      (first_letter == 'C') text->moveBy(-5,0);
            else if (first_letter == 'R') text->moveBy(3,0);
        }
        else
        {
            if      (first_letter == 'R') text->moveBy(0,3);
            text->moveBy(-15,-45);
        }
    }
}
