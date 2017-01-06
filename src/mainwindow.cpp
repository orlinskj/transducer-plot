#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <memory>
#include <sstream>
#include <utility>

#include <QFileDialog>

#include "view/transducerdelegate.h"
#include "view/plotitemsdelegate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);

    QObject::connect(ui_->actionFileOpen,
                     SIGNAL(triggered()),
                     this,
                     SLOT(file_open()) );

    // adding first empty chart to a tree view and make it current plot
    add_chart();
    setup_view();

    ui_->plotView->setContextMenuPolicy(Qt::CustomContextMenu);

    QObject::connect(ui_->plotView,
                     SIGNAL(customContextMenuRequested(const QPoint &)),
                     this,
                     SLOT(on_tree_view_context_menu(const QPoint &)));

}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::setup_view()
{
    ui_->transducerView->setModel(&transducer_model_);
    ui_->transducerView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_->transducerView->setItemDelegate(new ac::TransducerDelegate);

    ui_->plotView->setModel(&plot_model_);
    ui_->plotView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_->plotView->setHeaderHidden(true);
    ui_->plotView->setItemDelegate(new ac::PlotItemsDelegate);
}

void MainWindow::test_slot()
{

}

void MainWindow::on_tree_view_context_menu(const QPoint& point)
{
    QModelIndex index = ui_->plotView->indexAt(point);

    QMenu* context_menu = new QMenu;

    if (index.isValid()) {
        if (context_menu)
            context_menu->exec(ui_->plotView->mapToGlobal(point));
    }
}


void MainWindow::file_open()
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

void MainWindow::add_chart()
{
    auto plot = std::make_shared<ac::Plot>(Plot("Wykres"));
    QStandardItem* item = new QStandardItem(plot->get_name().c_str());
    item->setData(QVariant::fromValue(plot));
    plot_model_.appendRow(item);
}

void MainWindow::add_function(const Function& func)
{

}
