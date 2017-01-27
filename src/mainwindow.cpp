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
#include "model/treemodel/treeitem.h"
#include "model/transduceradapteritem.h"

#include "functiondialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);

    setup_view();
    seed();
    create_menus();
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
    plot_presenter_ = new ac::PlotPresenter(ui_->frame);
    plot_presenter_->view()->setContextMenuPolicy(Qt::CustomContextMenu);

}

void MainWindow::init_signals()
{
    QObject::connect(ui_->actionFileOpen, SIGNAL(triggered()),
                     this, SLOT(slot_file_open()) );

    QObject::connect(ui_->plotView, SIGNAL(customContextMenuRequested(const QPoint &)),
                     this, SLOT(slot_on_tree_view_context_menu(const QPoint &)));

    QObject::connect(plot_presenter_->view(), SIGNAL(customContextMenuRequested(const QPoint&)),
                     this, SLOT(slot_on_plot_view_context_menu(QPoint)));

    QObject::connect(ui_->plotAddButton, SIGNAL(clicked()),
                     this, SLOT(slot_add_new_plot()));

    QObject::connect(ui_->plotRemoveButton, SIGNAL(clicked()),
                     this, SLOT(slot_remove_plot()));

    QObject::connect(ui_->functionAddButton, SIGNAL(clicked()),
                     this, SLOT(slot_add_function()));

    QObject::connect(ui_->functionRemoveButton, SIGNAL(clicked()),
                     this, SLOT(slot_remove_function()));
}

void MainWindow::slot_on_tree_view_context_menu(const QPoint& point)
{
    QModelIndex index = ui_->plotView->indexAt(point);

    if (index.isValid()) {
        auto tree_item = index.data(TreeItemModel::Role).value<TreeItem*>();
        if (auto plot = dynamic_cast<ac::Plot*>(tree_item))
            plot_menu_.exec(ui_->plotView->mapToGlobal(point));
    }
}

void MainWindow::slot_on_plot_view_context_menu(const QPoint& point)
{
    plot_view_menu_.exec(plot_presenter_->view()->mapToGlobal(point));
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
        auto transducer = file_reader.read(filename.toLocal8Bit(),
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

void MainWindow::add_transducer(ac::Transducer* transducer)
{
    transducer_model_.append(new ac::TransducerAdapterItem(transducer));
}

void MainWindow::seed()
{
    FileReader file_reader;
    int status;
    std::vector<Transducer*> vec = { file_reader.read("/home/janek/test/2_P0.txt", &status),
                 file_reader.read("/home/janek/test/2_P1.txt", &status),
                 file_reader.read("/home/janek/test/2_P4.txt", &status) };

    for(auto it=vec.begin(); it!=vec.end(); it++)
        this->add_transducer(*it);
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
        auto item = selected.at(0).data(TreeItemModel::Role).value<TreeItem*>();
        if (auto plot = dynamic_cast<ac::Plot*>(item))
            plot->kill();
    }
}

void MainWindow::create_menus()
{
    QAction* show_plot = plot_menu_.addAction(tr("Pokaż wykres"));
    QAction* remove_plot = plot_menu_.addAction(tr("Usuń"));

    QObject::connect(show_plot,SIGNAL(triggered()),
                     this,SLOT(slot_show_plot()));
    QObject::connect(remove_plot,SIGNAL(triggered()),
                     this, SLOT(slot_remove_plot()));

    QAction* log_axis = plot_view_menu_.addAction(tr("Oś Y logarytmiczna"));
    QAction* linear_axis = plot_view_menu_.addAction(tr("Oś Y liniowa"));

    QObject::connect(log_axis,SIGNAL(triggered()),
                     plot_presenter_,SLOT(set_log_axis()));
    QObject::connect(linear_axis, SIGNAL(triggered()),
                     plot_presenter_, SLOT(set_linear_axis()));
}

void MainWindow::slot_add_function()
{
    auto selection = ui_->plotView->selectionModel();
    auto selected = selection->selectedIndexes();

    if(selected.length() == 1)
    {
        auto index = selected.at(0);
        auto item = index.data(TreeItemModel::Role).value<TreeItem*>();
        Plot* plot= nullptr;

        if (auto func = dynamic_cast<ac::Function*>(item))
            plot = dynamic_cast<Plot*>(func->parent());
        else if(auto plot_ptr = dynamic_cast<ac::Plot*>(item))
            plot = plot_ptr;

        if (plot)
        {
            auto f = new FunctionDialog(nullptr, &transducer_model_, plot);
            f->show();
        }
    }
}

void MainWindow::slot_remove_function()
{
    auto selection = ui_->plotView->selectionModel();
    auto selected = selection->selectedIndexes();

    if(selected.length() == 1)
    {
        auto item = selected.at(0).data(TreeItemModel::Role).value<TreeItem*>();
        if (auto func = dynamic_cast<ac::Function*>(item))
            func->kill();
    }
}

void MainWindow::slot_show_plot()
{
    auto selection = ui_->plotView->selectionModel();

    if ( selection->selectedIndexes().length() == 1)
    {
        auto item = selection->selectedIndexes().at(0).data(TreeItemModel::Role).value<TreeItem*>();
        if (auto plot = dynamic_cast<ac::Plot*>(item))
            plot_presenter_->show_plot(plot);
    }
}
