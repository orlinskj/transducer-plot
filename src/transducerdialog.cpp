#include "transducerdialog.h"
#include "ui_transducerdialog.h"

#include "viewmodel/treemodel/treeitemmodel.h"
#include "viewmodel/transducertableproxymodel.h"
#include "viewmodel/setitem.h"
#include "view/transducerdelegate.h"
#include "view/plotpresenter.h"

#include "io/csvexporthandler.h"
#include "io/pdfexporthandler.h"
#include "io/exportfilehandler.h"

#include "pathfinder.h"

#include <QComboBox>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QMessageBox>
#include <QDesktopServices>

#include <string>
#include <tuple>
#include <memory>

// windows does not support UTF in QLabel
#ifdef _WIN32
    #define SUBSCRIPT_P "p"
    #define SUBSCRIPT_S "s"
#elif
    #define SUBSCRIPT_P "ₚ"
    #define SUBSCRIPT_S "ₛ"
#endif

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

TransducerDialog::TransducerDialog(QWidget *parent, TreeItemModel* transducer_model, PlotPresenter* presenter) :
    QDialog(parent),
    ui(new Ui::TransducerDialog),
    transducer_table_model_(nullptr),
    transducer_model_(transducer_model),
    pixmap_(nullptr),
    presenter_(presenter)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);

    ui->graphicsViewModel->setScene(new QGraphicsScene);
    ui->graphicsViewModel->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsViewModel->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->transducerComboBox->setModel(transducer_model);
    ui->transducerComboBox->setItemDelegate(new TransducerDelegate);

    ui->tableView->verticalHeader()->setDefaultSectionSize(18);

    ui->tableViewModelParams->verticalHeader()->setDefaultSectionSize(20);
    ui->tableViewModelParams->setModel(&table_params_model_);
    ui->tableViewModelParams->horizontalHeader()->hide();

    ui->tableWidgetCop->verticalHeaderItem(0)->setText(QString("Co")+QString(SUBSCRIPT_P));

    // transducer list
    connect(ui->transducerComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [this](int index){ transducer_changed(dynamic_cast<TransducerItem*>(transducer_model_->child(index))); });

    // model parameters
    connect(ui->tableWidgetCop, &QTableWidget::cellChanged,
            this, [this](int row, int col){ Q_UNUSED(row); Q_UNUSED(col); this->recalc_model(); });

    connect(ui->fixedCopCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(fixed_capacity_checkbox(int)));

    // model type
    connect(ui->comboBoxModelType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &TransducerDialog::model_type_changed);

    // export behaviour
    connect(ui->exportTypecomboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &TransducerDialog::export_type_changed);

    connect(ui->pathPushButton, &QPushButton::clicked,
            this, [this](){ PathFinder::show(ui->pathLineEdit, tr("Przetwornik (*.pdf *.csv)")); });

    connect(ui->exportPushButton, &QPushButton::clicked,
            this, &TransducerDialog::export_transducer);

    // getting first transducer - and set as current if exists
    if (transducer_model_->rowCount(QModelIndex()))
        transducer_changed(dynamic_cast<TransducerItem*>(transducer_model_->child(0)));

    // triggering model type change
    ui->comboBoxModelType->currentIndexChanged(0);
    ui->exportTypecomboBox->currentIndexChanged(0);

}

TransducerDialog::~TransducerDialog()
{ }

void TransducerDialog::export_transducer()
{
    if (ui->transducerComboBox->currentIndex() < 0)
        return;

    recalc_model();

    auto transducer_item = dynamic_cast<TransducerItem*>(
                transducer_model_->child(ui->transducerComboBox->currentIndex()));

    std::unique_ptr<ExportFileHandler> export_handler;

    // PDF
    if (ui->exportTypecomboBox->currentIndex() == 0){
        PDFExportHandler::Options options;
        // set options
        options.model_data = true;
        options.plots = ui->plotsCheckBox->isChecked();
        options.tab_data = ui->tabDataExportCheckbox->isChecked();
        export_handler.reset(new PDFExportHandler(transducer_item, options, &solver_, presenter_));
    }
    // CSV
    else if(ui->exportTypecomboBox->currentIndex() == 1){
        CSVExportHandler::Options options;
        // set options
        // ...
        export_handler.reset(new CSVExportHandler(transducer_item, options, transducer_table_model_.get()));
    }
    else{
        // unrecognized option - ERROR!
        return;
    }

    QString path = QString::fromStdString(
                export_handler->path(QDir(ui->pathLineEdit->text()).absolutePath().toStdString()));
    bool save = false;

    // checking whether file exists
    if (QDir().exists(QDir(path).absolutePath())){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
                    this,
                    tr("Zastąp plik"),
                    tr("Plik o podanej ścieżce")+" "+path+" "+tr("istnieje. Nadpisać plik?"),
                    QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            save = true;
        }
    }
    else {
        save = true;
    }

    if (save){
        if (export_handler->save(ui->pathLineEdit->text().toStdString()) == 0){
            QMessageBox msg(
                        QMessageBox::Icon::Information,
                        "Sukces",
                        "Plik przetwornika został zpisany pod ścieżką "+path,
                        QMessageBox::Button::Close,
                        this);

            QPushButton* open_button = msg.addButton(tr("Otwórz plik"), QMessageBox::ActionRole);
            msg.exec();

            if (msg.clickedButton() == open_button) {
                // open file
                QDesktopServices::openUrl(QUrl(path));
            }
        }
        else{
            QMessageBox msg(
                        QMessageBox::Icon::Critical,
                        "Błąd eksportu",
                        "Błąd eksportu pliku przetwornika. Sprawdź czy posiadasz prawo zapisu pod podaną ścieżką.",
                        QMessageBox::Button::Ok,
                        this);
            msg.show();
        }
    }
}

void TransducerDialog::set_tab(int tab)
{
    ui->tabWidget->setCurrentIndex(tab);
}

void TransducerDialog::transducer_changed(TransducerItem* t)
{
    // reset sets model
    ui->tableView->setModel(nullptr);
    transducer_table_model_.reset(new TransducerTableProxyModel(t));
    ui->tableView->setModel(transducer_table_model_.get());

    // change export default filename
    ui->pathLineEdit->setText(QString::fromStdString(t->to_string()));
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

    std::pair<bool, SolverType> input_capacity = std::make_pair(false,0);
    auto Cop_item = ui->tableWidgetCop->item(0,0);
    if (ui->fixedCopCheckBox->isChecked() == true && Cop_item)
    {
        std::string Co_cell = Cop_item->data(Qt::DisplayRole).toString().toStdString();

        input_capacity.first = true;
        input_capacity.second = Unit::Capacity.value_from_repr<SolverType>(Co_cell);
    }

    table_params_model_.clear();

    if (!transducer_item)
        return;

    solver_.solve(*(transducer_item->value()),input_capacity);

    // setting cells' values
    std::vector<std::tuple<qreal,Unit>> vals = {
        std::make_tuple(solver_.f(type),Unit::from_symbol("f")),
        std::make_tuple(solver_.Co(type),Unit::from_symbol("C")),
        std::make_tuple(solver_.R(type),Unit::from_symbol("R")),
        std::make_tuple(solver_.L(type),Unit::from_symbol("L")),
        std::make_tuple(solver_.C(type),Unit::from_symbol("C")),
        std::make_tuple(solver_.Q(type),Unit::None),
        std::make_tuple(solver_.QQ(),Unit::None),
        std::make_tuple(solver_.keff(),Unit::None),
        std::make_tuple(solver_.k33(),Unit::None),
        std::make_tuple(solver_.k(),Unit::None)
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

    QStringList header_items = {
        "f",    "Co",   "R",
        "L",    "C",    "Q",    "",
        "Q",    "keff", "k33",  "k"
    };

    QString script_index(SUBSCRIPT_P);

    if (type == BVDSolver::Type::Series){
        script_index = QString(SUBSCRIPT_S);
    }

    for (auto it = header_items.begin(); it != header_items.begin()+6; ++it)
        *it += script_index;

    table_params_model_.setVerticalHeaderLabels(header_items);
}

void TransducerDialog::fixed_capacity_checkbox(int state)
{
    if (state == Qt::Checked)
        ui->tableWidgetCop->setEnabled(true);
    else
        ui->tableWidgetCop->setEnabled(false);

    recalc_model();
}

void TransducerDialog::export_type_changed(int index)
{
    // PDF
    if (index == 0){
        ui->tabDataExportCheckbox->setDisabled(true);
        ui->tabDataExportCheckbox->setChecked(false);
        //ui->plotsExportComboBox->setEnabled(true);
        ui->plotsCheckBox->setEnabled(true);
    }
    // CSV
    else if(index == 1) {
        ui->tabDataExportCheckbox->setDisabled(true);
        ui->tabDataExportCheckbox->setChecked(true);

        //ui->plotsExportComboBox->setDisabled(true);
        //ui->plotsExportComboBox->setCurrentIndex(0);
        ui->plotsCheckBox->setDisabled(true);
        ui->plotsCheckBox->setChecked(false);

    }
    else{
        qDebug() << "Unrecognized export type index in QComboBox";
    }
}

void TransducerDialog::model_type_changed(int index)
{
    ui->graphicsViewModel->scene()->clear();
    pixmap_ = nullptr;

    // series BVD model
    if (index == 0)
    {
        pixmap_ = new QGraphicsPixmapItem(QPixmap(":/icons/bvd-series.svg"));
        pixmap_->setTransformationMode(Qt::SmoothTransformation);
        ui->graphicsViewModel->scene()->addItem(pixmap_);
        place_pixmap_labels(labels_series);
    }
    // parallel BVD model
    else if (index == 1)
    {
        pixmap_ = new QGraphicsPixmapItem(QPixmap(":/icons/bvd-parallel.svg"));
        pixmap_->setTransformationMode(Qt::SmoothTransformation);
        ui->graphicsViewModel->scene()->addItem(pixmap_);
        place_pixmap_labels(labels_parallel);
    }
    else{
        return;
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
/*
FileOpenButton::FileOpenButton(QWidget *parent, QString path) :
    QPushButton(parent), path_(path)
{
    setText("Otwórz plik");
}

bool FileOpenButton::hitButton(const QPoint &pos) const
{
    QDesktopServices::openUrl(QUrl(path_));
}*/
