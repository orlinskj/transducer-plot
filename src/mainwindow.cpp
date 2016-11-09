#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->actionFileOpen,
                     SIGNAL(triggered()),
                     this,
                     SLOT(fileOpen()) );

    QStandardItem *parentItem = model.invisibleRootItem();
    QStandardItem *item = new QStandardItem(QIcon(":/icons/v-chart.png"), "Wykres 1");
    parentItem->appendRow(item);

    current_plot_qindex = item->index();
    plots.push_back( unique_ptr<PlotQt>(new PlotQt(getPlotHandle())));
    current_plot = plots.front().get();

    ui->treeView->setHeaderHidden(true);
    ui->treeView->setModel(&model);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->treeView,
                     SIGNAL(customContextMenuRequested(const QPoint &)),
                     this,
                     SLOT(onTreeViewContextMenu(const QPoint &)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QCustomPlot* MainWindow::getPlotHandle()
{
    return ui->plot;
}

void MainWindow::testSlot()
{

}

void MainWindow::onTreeViewContextMenu(const QPoint& point)
{
    QModelIndex index = ui->treeView->indexAt(point);

    QMenu* contextMenu = new QMenu;
    contextMenu->addAction(tr("Ustaw jako dziedzinę"), this, SLOT(testSlot()));
    contextMenu->addAction(tr("Ustaw jako przeciwdziedzinę"), this, SLOT(testSlot()));

    if (index.isValid())
    {
        contextMenu->exec(ui->treeView->mapToGlobal(point));
    }
}


void MainWindow::fileOpen()
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
        unique_ptr<DataSet> dataset = file_reader.read(filename.toLocal8Bit(),
                                                       &status);

        if (dataset)
        {
            this->addDataSet(std::move(*dataset));
            ui->statusBar->showMessage(tr("Wczytano plik ")+filename);

            stringstream ss;
            ss << "Ilość grafów: " << ui->plot->graphCount();
            ui->label->setText( ss.str().c_str() );
        }
        else
        {
            ui->statusBar->showMessage(
                        tr("Nie udało się wczytać pliku ")+filename);
        }
    }

}

void MainWindow::addDataSet(DataSet&& dataset)
{
    // dodajemy do wykresu obiekt zbioru danych
    QStandardItem* plot_item = model.itemFromIndex(current_plot_qindex);
    QStandardItem* item = new QStandardItem(dataset.getName().c_str());
    QList<QStandardItem*> items = { item };
    plot_item->appendRow(items);

    // dodajemy do obiektu zbioru danych wartości
    items.clear();
    QStandardItem* sets_item = new QStandardItem("Zbiory danych");
    QStandardItem* params_item = new QStandardItem("Prametry zastępcze");
    items = { sets_item, params_item };
    item->appendColumn(items);

    items.clear();

    auto series = dataset.getSeries();
    for(auto it=series.begin(); it!=series.end(); it++)
    {
        items.append(new QStandardItem(it->getLabel().c_str()));
    }
    sets_item->appendColumn(items);

    current_plot->addDataSet(std::move(dataset));
}

void MainWindow::configPlot()
{
    QCustomPlot* plot = getPlotHandle();

    plot->setInteraction(QCP::iRangeDrag, true);
    plot->setInteraction(QCP::iRangeZoom, true);

    plot->xAxis->setLabel("x");
    plot->yAxis->setLabel("y");

    // set axes ranges, so we see all data:
    plot->xAxis->setRange(-1, 1);
    plot->yAxis->setRange(-1, 1);

    plot->yAxis->setScaleType(QCPAxis::ScaleType::stLogarithmic);
    plot->setInteraction(QCP::iSelectPlottables, true);

    plot->replot();
}

void MainWindow::addFunction(const Function& func)
{
    current_plot->addFunction(func);
}
