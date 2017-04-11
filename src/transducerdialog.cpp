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
#include <tuple>
#include <boost/filesystem.hpp>
#include "model/solver.h"

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
            this, [this](int index){
        set_transducer(dynamic_cast<TransducerItem*>(transducer_model_->child(index)));
    });

    connect(ui->tableWidgetCop, &QTableWidget::cellChanged,
            this, [this](int row, int col){
        this->recalc_model();
    });

    connect(ui->fixedCopCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(fixed_capacity_checkbox(int)));

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

    ui->tableViewModelParams->verticalHeader()->setDefaultSectionSize(20);
    ui->tableViewModelParams->setModel(&table_params_model_);
    ui->tableViewModelParams->horizontalHeader()->hide();
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

void TransducerDialog::recalc_model()
{
    BVDSolver::Type type;
    if (ui->comboBoxModelType->currentIndex() == 0)
        type = BVDSolver::Type::Series;
    else
        type = BVDSolver::Type::Parallel;

    auto transducer_item = dynamic_cast<TransducerItem*>(
                transducer_model_->child(ui->transducerComboBox->currentIndex()));

    boost::optional<SolverType> input_capacity = boost::none;
    auto Cop_item = ui->tableWidgetCop->item(0,0);
    if (ui->fixedCopCheckBox->isChecked() == true && Cop_item)
    {
        std::string Co_cell = Cop_item->data(Qt::DisplayRole).toString().toStdString();
        input_capacity = Unit::from_symbol("C").value_from_repr<SolverType>(Co_cell);
        qDebug() << "input capacity = " << *input_capacity;
    }

    table_params_model_.clear();

    QStringList header_items = {
        "f",    "Co",   "R",
        "L",    "C",    "Q",    "",
        "Q",    "keff", "k33",  "k"
    };

    QString script_index = "ₚ";
    if (type == BVDSolver::Type::Series)
        script_index = "ₛ";

    for (auto it = header_items.begin(); it != header_items.begin()+6; ++it)
        *it += script_index;

    table_params_model_.setVerticalHeaderLabels(header_items);

    if (!transducer_item)
        return;

    BVDSolver solver;
    solver.solve(*(transducer_item->value()),input_capacity);

    // setting cells' values
    std::vector<std::tuple<qreal,Unit>> vals = {
        std::make_tuple(solver.f(type),Unit::from_symbol("f")),
        std::make_tuple(solver.Co(type),Unit::from_symbol("C")),
        std::make_tuple(solver.R(type),Unit::from_symbol("R")),
        std::make_tuple(solver.L(type),Unit::from_symbol("L")),
        std::make_tuple(solver.C(type),Unit::from_symbol("C")),
        std::make_tuple(solver.Q(type),Unit::None),
        std::make_tuple(solver.QQ(),Unit::None),
        std::make_tuple(solver.keff(),Unit::None),
        std::make_tuple(solver.k33(),Unit::None),
        std::make_tuple(solver.k(),Unit::None)
    };

    size_t i = 0;
    for(auto val : vals)
    {
        auto str = QString::number(std::get<0>(val));
        if (i++ < 5)
            str = QString::fromStdString( std::get<1>(val).nice_repr(std::get<0>(val)));
        table_params_model_.appendRow(new QStandardItem(str));
    }
    table_params_model_.insertRow(6,new QStandardItem(""));


}

void TransducerDialog::fixed_capacity_checkbox(int state)
{
    if (state == Qt::Checked)
        ui->tableWidgetCop->setEnabled(true);
    else
        ui->tableWidgetCop->setEnabled(false);

    recalc_model();
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

    recalc_model();
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
