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
#include "viewmodel/treemodel/treeitem.h"
#include "viewmodel/transduceritem.h"
#include "viewmodel/functionitem.h"

#include "functiondialog.h"
#include "transducerdialog.h"
#include "aboutdialog.h"
#include "pdfprinter.h"
//#include "screenshotdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow),
    plot_presenter_(new PlotPresenter(&plot_store_)),
    transducer_dialog_(nullptr),
    about_dialog_(nullptr),
    screenshot_form_(nullptr)
{
    ui_->setupUi(this);

    setup_view();
    init_signals();

    // emitting resize event
    show();
    QResizeEvent r(size(),size());
    resizeEvent(&r);
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
    ui_->transducerView->setItemDelegate(new TransducerDelegate);
    ui_->transducerView->setDragEnabled(true);

    ui_->plotView->setModel(&plot_store_);
    ui_->plotView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_->plotView->setHeaderHidden(true);
    //ui_->plotView->setItemDelegate(new PlotItemsDelegate);
    ui_->plotView->setContextMenuPolicy(Qt::CustomContextMenu);

    // plot presenter configuration
    plot_presenter_->setContextMenuPolicy(Qt::CustomContextMenu);

    // frame for chart view configuration
    QBoxLayout* layout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
    layout->addWidget(plot_presenter_);
    layout->setContentsMargins(0,0,0,0);
    ui_->frame->setLayout(layout);

    // auto s = ui_->frame->size();
    screenshot_form_ = new ScreenshotForm(ui_->frame, plot_presenter_);
    screenshot_form_->raise();

}

void MainWindow::init_signals()
{
    // buttons
    connect(ui_->plotAddButton, &QPushButton::clicked,
            this, &MainWindow::add_plot);

    connect(ui_->plotRemoveButton, &QPushButton::clicked,
            this, &MainWindow::remove_plot);

    connect(ui_->functionAddButton, &QPushButton::clicked,
            this, &MainWindow::add_function);

    connect(ui_->functionRemoveButton, &QPushButton::clicked,
            this, &MainWindow::remove_function);

    connect(ui_->transducerAddButton, &QPushButton::clicked,
            this, &MainWindow::add_transducer);

    connect(ui_->transducerRemoveButton, &QPushButton::clicked,
            this, &MainWindow::remove_transducer);

    // context menus
    connect(ui_->plotView, &QGraphicsView::customContextMenuRequested,
            this, &MainWindow::plot_view_menu);

    QAction* action_show_plot = plot_menu_.addAction(tr("Pokaż wykres"));
    QAction* action_remove_plot = plot_menu_.addAction(tr("Usuń"));

    connect(action_show_plot, &QAction::triggered,
            this, &MainWindow::show_plot);

    connect(action_remove_plot, &QAction::triggered,
            this, &MainWindow::remove_plot);

    // menu bar
    connect(ui_->actionTransducerToPdf, &QAction::triggered,
            this, [this](){ show_transducer_dialog(2); });

    connect( ui_->actionPlotScreenshot, &QAction::triggered,
             this, [this](){ screenshot_form_->show(); });

    connect( ui_->actionTransducerTableData, &QAction::triggered,
             this, [this](){ show_transducer_dialog(0); });

    connect( ui_->actionTransducerModels, &QAction::triggered,
             this, [this](){ show_transducer_dialog(1); });

    connect( ui_->actionHelpAbout, &QAction::triggered,
             this, [this]{ show_about_dialog(); });
}

void MainWindow::plot_view_menu(const QPoint& point)
{
    QModelIndex index = ui_->plotView->indexAt(point);

    if (index.isValid()) {
        auto tree_item = index.data(TreeItemModel::Role).value<TreeItem*>();
        if (/*auto plot = */dynamic_cast<PlotItem*>(tree_item))
            plot_menu_.exec(ui_->plotView->mapToGlobal(point));
    }
}


void MainWindow::add_transducer()
{
    QString filename = QFileDialog::getOpenFileName(
                this, tr("Otwórz plik z danymi"),
                ".", tr("Pliki przetwornika (*.txt *.csv);;Wszystkie pliki (*.*)"));

    if (filename.isEmpty())
        return;

    auto transducer = Loader().load(filename.toStdString());
    if (transducer)
    {
        transducer_model_.append(new TransducerItem(transducer));
        ui_->statusBar->showMessage(tr("Wczytano plik")+" "+filename);
    }
    else
        ui_->statusBar->showMessage(tr("Nie udało się wczytać pliku ")+" "+filename);
}

void MainWindow::remove_transducer()
{
    auto selected = ui_->transducerView->selectionModel()->selectedIndexes();
    if (selected.count() > 0){
        auto index = selected.first();
        auto tree_item = transducer_model_.data(index, TreeItem::Role).value<TreeItem*>();
        auto transducer_item = dynamic_cast<TransducerItem*>(tree_item);

        // removing all functions with this transducer
        for(const auto& plot : static_cast<TreeItem&>(plot_store_).children()){
            for(const auto& func : plot->children()){
                auto func_item = dynamic_cast<FunctionItem*>(func);
                if (func_item->value()->transducer() == transducer_item->value()){
                    plot->remove(func_item);
                }
            }

            if (plot->children_count() == 0){
                static_cast<TreeItem&>(plot_store_).remove(plot);
            }
        }

        transducer_model_.remove(transducer_item);
        ui_->statusBar->showMessage(tr("Przetwornik usunięty"));
    }

}

/*void MainWindow::seed()
{
    std::vector<std::string> files;
    #ifdef __linux__
    files.push_back("/home/janek/test/2_P0.txt");
    files.push_back("/home/janek/test/2_P1.txt");
    files.push_back("/home/janek/test/2_P4.txt");
    #elif _WIN32
    files.push_back("C:\\test\\2_P0.txt");
    files.push_back("C:\\test\\2_P1.txt");
    files.push_back("C:\\test\\2_P4.txt");
    #endif

    for(auto file : files)
    {
        int err = 1;
        auto transducer = Loader().load(file);
        if (transducer && !err)
            this->add_transducer(transducer);
    }

    // some plot, with some function ...
    this->slot_add_new_plot();

    auto plot = dynamic_cast<PlotItem*>(plot_store_.child(0));
    auto tadapter = dynamic_cast<TransducerItem*>(transducer_model_.child(0));
    if (tadapter)
    {
        auto transducer = (*tadapter)();
        auto func = new Function(transducer,&transducer->get_sets()[0], &transducer->get_sets()[1]);
        plot->append(new FunctionItem(func));
    }
    plot_presenter_->show_plot(plot);
}*/

void MainWindow::add_plot()
{
    plot_store_.append(new PlotItem);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    auto screenshot_height = screenshot_form_->size().height();
    auto frame_width = ui_->frame->size().width();
    screenshot_form_->resize(frame_width-12,screenshot_height);
}

void MainWindow::remove_plot()
{
    auto selection = ui_->plotView->selectionModel();
    auto selected = selection->selectedIndexes();

    if(selected.length() == 1)
    {
        auto item = selected.at(0).data(TreeItemModel::Role).value<TreeItem*>();
        if (auto plot = dynamic_cast<PlotItem*>(item))
            plot->kill();
    }
}

void MainWindow::show_transducer_dialog(int tab)
{

    if (!transducer_dialog_)
    {
        transducer_dialog_ = new TransducerDialog(this,&transducer_model_, plot_presenter_);
        QObject::connect(transducer_dialog_, &QDialog::destroyed,
                         this, [this](){ transducer_dialog_ = nullptr; });
    }
    transducer_dialog_->set_tab(tab);
    transducer_dialog_->show();
    transducer_dialog_->setFocus();
}

void MainWindow::show_about_dialog()
{
    if (!about_dialog_)
    {
        about_dialog_ = new AboutDialog(this);
        QObject::connect(about_dialog_, &QDialog::destroyed,
                         this, [this](){ about_dialog_ = nullptr; });
    }
    about_dialog_->show();
}

void MainWindow::add_function()
{
    auto selection = ui_->plotView->selectionModel();
    auto selected = selection->selectedIndexes();

    if(selected.length() == 1)
    {
        auto index = selected.at(0);
        auto item = index.data(TreeItemModel::Role).value<TreeItem*>();
        PlotItem* plot = nullptr;

        if (auto func = dynamic_cast<FunctionItem*>(item))
            plot = dynamic_cast<PlotItem*>(func->parent());
        else if(auto plot_ptr = dynamic_cast<PlotItem*>(item))
            plot = plot_ptr;

        if (plot)
        {
            auto f = new FunctionDialog(this, &transducer_model_, plot);
            f->show();
        }
    }
}

void MainWindow::remove_function()
{
    auto selection = ui_->plotView->selectionModel();
    auto selected = selection->selectedIndexes();

    if(selected.length() == 1)
    {
        auto item = selected.at(0).data(TreeItemModel::Role).value<TreeItem*>();
        if (auto func = dynamic_cast<FunctionItem*>(item))
            func->kill();
    }
}

void MainWindow::show_plot()
{
    auto selection = ui_->plotView->selectionModel();

    if ( selection->selectedIndexes().length() == 1)
    {
        auto item = selection->selectedIndexes().at(0).data(TreeItemModel::Role).value<TreeItem*>();
        if (auto plot = dynamic_cast<PlotItem*>(item))
            plot_presenter_->show_plot(plot);
    }
}
