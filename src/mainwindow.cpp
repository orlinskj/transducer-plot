#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <memory>
#include <sstream>
#include <utility>

#include <QFileDialog>
#include <QLineSeries>
#include <QPushButton>
#include <QBoxLayout>
#include <QGraphicsLayout>

#include "view/transducerdelegate.h"
//#include "view/plotitemsdelegate.h"
#include "model/treemodel/treenodeitem.h"

#include "functiondialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);

    setup_view();
    seed();
    init_signals();
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::setup_view()
{
    // transducer view configuration
    ui_->transducerView->setModel(&transducer_model_);
    ui_->transducerView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_->transducerView->setItemDelegate(new ac::TransducerDelegate);
    ui_->transducerView->setDragEnabled(true);

    ui_->plotView->setModel(&plot_store_);
    ui_->plotView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_->plotView->setHeaderHidden(true);
    //ui_->plotView->setItemDelegate(new ac::PlotItemsDelegate);

    ui_->plotView->setContextMenuPolicy(Qt::CustomContextMenu);

    // frame for chart view configuration
    QBoxLayout* layout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
    layout->setContentsMargins(0,0,0,0);
    ui_->frame->setLayout(layout);

    // plot presenter configuration
    plot_presenter_ = ac::PlotPresenter(ui_->frame);

}

void MainWindow::init_signals()
{
    QObject::connect(ui_->actionFileOpen, SIGNAL(triggered()),
                     this, SLOT(slot_file_open()) );

    QObject::connect(ui_->plotView, SIGNAL(customContextMenuRequested(const QPoint &)),
                     this, SLOT(slot_on_tree_view_context_menu(const QPoint &)));

    QObject::connect(ui_->plotAddButton, SIGNAL(clicked()),
                     this, SLOT(slot_add_new_plot()));

    QObject::connect(ui_->plotRemoveButton, SIGNAL(clicked()),
                     this, SLOT(slot_remove_plot()));

    QObject::connect(ui_->functionAddButton, SIGNAL(clicked()),
                     this, SLOT(slot_add_function()));

    QObject::connect(ui_->functionRemoveButton, SIGNAL(clicked()),
                     this, SLOT(slot_remove_function()));
}

void MainWindow::slot_test()
{

}

void MainWindow::slot_on_tree_view_context_menu(const QPoint& point)
{
    QModelIndex index = ui_->plotView->indexAt(point);

    QMenu* context_menu = new QMenu;

    if (index.isValid()) {
        if (context_menu)
            context_menu->exec(ui_->plotView->mapToGlobal(point));
    }
}


void MainWindow::slot_file_open()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Otwórz plik z danymi"),
                ".",
                tr("Pliki tekstowe (*.txt);;Wszystkie pliki (*.*)"));

    if (!filename.isEmpty())
    {
        int status;
        FileReader file_reader;
        std::shared_ptr<Transducer> transducer = file_reader.read(filename.toLocal8Bit(),
                                                       &status);
        if (transducer)
        {
            this->add_transducer(transducer);
            ui_->statusBar->showMessage(tr("Wczytano plik ")+filename);
        }
        else
        {
            ui_->statusBar->showMessage(
                        tr("Nie udało się wczytać pliku ")+filename);
        }
    }

}

void MainWindow::add_transducer(std::shared_ptr<Transducer> transducer)
{
    QStandardItem* item = new QStandardItem(transducer->get_name().c_str());
    QVariant var = QVariant::fromValue<std::shared_ptr<Transducer>>(transducer);
    item->setData(var);
    transducer_model_.appendRow(item);
}

void MainWindow::seed()
{
    /*QLineSeries *series = new QLineSeries();
    series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);
    *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Simple line chart example");
    chart->layout()->setContentsMargins(0,0,0,0);

    chartView->setChart(chart);*/
}

void MainWindow::slot_add_new_plot()
{
    plot_store_.append(new ac::Plot());
}

void MainWindow::slot_remove_plot()
{
    auto selection = ui_->plotView->selectionModel();
    auto selected = selection->selectedIndexes();

    if(selected.length() == 1)
    {
        auto item = selected.at(0).data(TreeItemModel::Role).value<TreeNodeItem*>();
        if (auto plot = dynamic_cast<ac::Plot*>(item))
            plot->kill();
    }
}

void MainWindow::slot_add_function()
{
    /*auto selection = ui_->plotView->selectionModel();
    auto selected = selection->selectedIndexes();

    if(selected.length() == 1)
    {
        auto index = selected.at(0);
        auto item = index.data(TreeItemModel::Role).value<TreeNodeItem*>();
        if (auto func = dynamic_cast<ac::Function*>(item))
            func->parent()->append(new ac::Function());
        else if(auto plot = dynamic_cast<ac::Plot*>(item))
            plot->append(new ac::Function());
    }*/

    auto dialog = new FunctionDialog();
    dialog->show();
}

void MainWindow::slot_remove_function()
{
    auto selection = ui_->plotView->selectionModel();
    auto selected = selection->selectedIndexes();

    if(selected.length() == 1)
    {
        auto item = selected.at(0).data(TreeItemModel::Role).value<TreeNodeItem*>();
        if (auto func = dynamic_cast<ac::Function*>(item))
            func->kill();
    }
}
