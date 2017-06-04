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
#include <QDesktopServices>

#include "view/transducerdelegate.h"
#include "view/plotitemdelegate.h"
#include "viewmodel/treemodel/treeitem.h"
#include "viewmodel/transduceritem.h"
#include "viewmodel/functionitem.h"

#include "functiondialog.h"
#include "transducerdialog.h"
#include "aboutdialog.h"
#include "functionform.h"
#include "error.h"
//#include "screenshotdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow),
    plot_presenter_(new PlotPresenter(&plot_store_)),
    transducer_dialog_(nullptr),
    about_dialog_(nullptr),
    screenshot_form_(nullptr),
    previous_stacked_index_(0)
{
    ui_->setupUi(this);

    setup_view();
    init_signals();

    show();
    // emitting resize event - it will scale properly screenshotform
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

    ui_->plotView->setModel(&plot_store_);
    ui_->plotView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_->plotView->setItemDelegate(new PlotItemDelegate(ui_->plotView, plot_presenter_));
    ui_->plotView->setContextMenuPolicy(Qt::CustomContextMenu);

    // plot presenter configuration
    plot_presenter_->setContextMenuPolicy(Qt::CustomContextMenu);

    // function form
    auto ff_layout = new QHBoxLayout();
    ff_layout->setMargin(0);
    ff_layout->setSpacing(0);
    auto ff_parent = ui_->stackedWidget->widget(FunctionFormWidgetIndex);
    function_form_ = new FunctionForm(
                ff_parent,ui_->transducerView,
                ui_->plotView,
                plot_presenter_,
                [this](PlotItem* plot){
                    ui_->stackedWidget->setCurrentIndex(PlotPresenterWidgetIndex);
                    plot_presenter_->show_plot(plot);
                    ui_->statusBar->showMessage(tr("Dodano nową funkcję"));
                },
                [this](){ ui_->stackedWidget->setCurrentIndex(previous_stacked_index_); });
    ff_layout->addWidget(function_form_);
    ff_parent->setLayout(ff_layout);

    // frame for chart view configuration
    QBoxLayout* layout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
    layout->addWidget(plot_presenter_);
    layout->setContentsMargins(0,0,0,0);
    ui_->frame->setLayout(layout);

    // auto s = ui_->frame->size();
    screenshot_form_ = new ScreenshotForm(ui_->frame, plot_presenter_);
    screenshot_form_->raise();

    // width constraint for buttons (scale them nicely and cross-platform according to text width)
    QFontMetrics fm(ui_->functionAddButton->font());
    ui_->functionAddButton->setMaximumWidth(fm.width(tr("funkcja"))+28);
    ui_->plotAddButton->setMaximumWidth(fm.width(tr("wykres"))+28);
    ui_->transducerAddButton->setMaximumWidth(fm.width(tr("przetwornik"))+28);
}

void MainWindow::init_signals()
{
    // buttons
    connect(ui_->plotAddButton, &QPushButton::clicked,
            this, &MainWindow::add_plot);

    connect(ui_->functionAddButton, &QPushButton::clicked,
            this, &MainWindow::add_function);

    connect(ui_->transducerAddButton, &QPushButton::clicked,
            this, &MainWindow::add_transducer);

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

    connect(ui_->actionPlotScreenshot, &QAction::triggered,
            this, [this](){ screenshot_form_->show(); });

    connect(ui_->actionTransducerTableData, &QAction::triggered,
            this, [this](){ show_transducer_dialog(0); });

    connect(ui_->actionTransducerModels, &QAction::triggered,
            this, [this](){ show_transducer_dialog(1); });

    connect(ui_->actionHelpAbout, &QAction::triggered,
            this, [this]{ show_about_dialog(); });

    connect(ui_->actionHelpHelp, &QAction::triggered,
            this, &MainWindow::show_help);

    // mouse actions

    connect(ui_->plotView, &QListView::doubleClicked,
            this, &MainWindow::plot_view_double_clicked);

    // plot presenter updates plot list view
    connect(plot_presenter_, &PlotPresenter::plot_showed,
            this, [this](PlotItem* plot, PlotItem* prev){
        if (plot)
            ui_->plotView->update(plot_store_.index(plot));
        if (prev)
            ui_->plotView->update(plot_store_.index(prev));
    });

    connect(&transducer_model_, &TreeItemModel::rowsAboutToBeRemoved,
            this, &MainWindow::transducer_to_be_removed);
}

void MainWindow::plot_view_menu(const QPoint& point)
{
    QModelIndex index = ui_->plotView->indexAt(point);

    if (index.isValid()) {
        auto tree_item = index.data(TreeItemModel::Role).value<TreeItem*>();
        if (dynamic_cast<PlotItem*>(tree_item))
            plot_menu_.exec(ui_->plotView->mapToGlobal(point));
    }
}

void MainWindow::plot_view_double_clicked(const QModelIndex& index){
    if (index != QModelIndex()){
        ui_->stackedWidget->setCurrentIndex(PlotPresenterWidgetIndex);
        auto plot_item = PlotItem::from_qmodelindex(index);

        if (plot_item)
            plot_presenter_->show_plot(plot_item);
        else
            plot_presenter_->show_plot(PlotItem::from_qmodelindex(index.parent()));

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
    transducer_model_.append(new TransducerItem(transducer));
    ui_->statusBar->showMessage(tr("Wczytano plik")+" "+filename);
}

void MainWindow::transducer_to_be_removed(const QModelIndex &parent, int first, int last)
{
    if (last - first != 0){
        qDebug() << "batch delete can lead to undefined behaviour :)";
    }

    auto index = transducer_model_.index(first,0,parent);
    auto tree_item = transducer_model_.data(index, TreeItem::Role).value<TreeItem*>();
    auto transducer_item = dynamic_cast<TransducerItem*>(tree_item);

    // removing all functions for this transducer
    auto& plots = static_cast<TreeItem&>(plot_store_).children();
    auto plot_it = plots.begin();

    while (plot_it != plots.end()){
        bool removed = false;

        auto& funcs = (*plot_it)->children();
        auto func_it = funcs.begin();

        while (func_it != funcs.end()){
            auto func_item = dynamic_cast<FunctionItem*>(*func_it);
            if (func_item->value()->transducer() == transducer_item->value()){
                func_it = (*plot_it)->remove(func_item);
                removed = true;
            }
            else{
                ++func_it;
            }
        }

        if ((*plot_it)->children_count() == 0 && removed){
            plot_it = static_cast<TreeItem&>(plot_store_).remove(*plot_it);
        }
        else{
            ++plot_it;
        }
    }
    ui_->statusBar->showMessage(tr("Przetwornik usunięty"));
}

void MainWindow::add_plot()
{
    plot_store_.append(new PlotItem);
    ui_->statusBar->showMessage(tr("Dodano nowy wykres"));
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
    if (plot_store_.children_count() == 0){
        add_plot();
    }
    function_form_->select_default();
    previous_stacked_index_ = PlotPresenterWidgetIndex;
    ui_->stackedWidget->setCurrentIndex(FunctionFormWidgetIndex);

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

void MainWindow::show_help()
{
    QString url = "https://github.com/orlinskj/transducer-plot/blob/master/doc/user-manual.md";
    bool opened = false;

    if (QDir().exists("user-manual.pdf")){
        opened = QDesktopServices::openUrl(QUrl("user-manual.pdf"));
    }

    if (!opened){
        opened = QDesktopServices::openUrl(QUrl(url));
    }

    if (!opened){
        throw Error((tr("Nie udało się otworzyć pomocy, zajrzyj na ")+url).toStdString(),Error::Type::Info);
    }
}
