#include "mainwindow.h"
#include "ui_mainwindow.h"

/*PlotQtPtrWrapper::PlotQtPtrWrapper(PlotQt* p)
{
    ptr.reset(p);
}

PlotQtPtrWrapper::PlotQtPtrWrapper(const PlotQtPtrWrapper& p)
{
    PlotQt* plot = new PlotQt(*(p.ptr.get()));
    ptr.reset(plot);
}*/

Q_DECLARE_METATYPE(DataSet*)
Q_DECLARE_METATYPE(Plot)
Q_DECLARE_METATYPE(Plot*)
Q_DECLARE_SMART_POINTER_METATYPE(std::unique_ptr)
Q_DECLARE_METATYPE(std::unique_ptr<Plot>)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->actionFileOpen,
                     SIGNAL(triggered()),
                     this,
                     SLOT(fileOpen()) );

    // adding first empty chart to a tree view and make it current plot
    addChart(true);

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

QCustomPlot* MainWindow::getPlotWidget()
{
    return ui->plot;
}

void MainWindow::testSlot()
{

}

void MainWindow::onTreeViewContextMenu(const QPoint& point)
{
    QModelIndex index = ui->treeView->indexAt(point);

    QMenu* context_menu = new QMenu;

    if (index.isValid()) {
        switch (index.data(ac::TreeView::ItemRole).toInt()) {
        case static_cast<int>(ac::TreeView::Item::Set):
            context_menu->addAction(tr("Ustaw jako dziedzinę"), this, SLOT(testSlot()));
            context_menu->addAction(tr("Ustaw jako przeciwdziedzinę"), this, SLOT(testSlot()));
            break;

        case static_cast<int>(ac::TreeView::Item::Chart):
            context_menu->addAction(tr("Dodaj zbiór danych..."), this, SLOT(testSlot()));
            context_menu->addAction(tr("Usuń wykres"), this, SLOT(testSlot()));
            break;

        case static_cast<int>(ac::TreeView::Item::DataSet):
            //auto ptr = index.data(ac::TreeView::PointerRole).value<DataSet*>();
            context_menu->addAction(tr("Usuń"));
            break;

        default:
            delete context_menu;
            context_menu = nullptr;
            break;

        }

        if (context_menu)
            context_menu->exec(ui->treeView->mapToGlobal(point));
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
    auto plot_item = model.itemFromIndex(current_plot_qindex);
    auto item = ac::TreeView::createItem(ac::TreeView::Item::DataSet, dataset.getName().c_str());
    QList<QStandardItem*> items = { item };
    plot_item->appendRow(items);

    // dodajemy do obiektu zbioru danych wartości
    items.clear();
    auto sets_item = ac::TreeView::createItem(ac::TreeView::Item::SetContainer);
    auto params_item = ac::TreeView::createItem(ac::TreeView::Item::AlternativeModelContainer);
    items = { sets_item, params_item };
    item->appendColumn(items);

    items.clear();
    auto serial_item = ac::TreeView::createItem(ac::TreeView::Item::AlternativeModelSerial);
    auto parallel_item = ac::TreeView::createItem(ac::TreeView::Item::AlternativeModelParallel);
    items = { serial_item, parallel_item };
    params_item->appendColumn(items);

    items.clear();

    auto series = dataset.getSeries();
    for(auto it=series.begin(); it!=series.end(); it++)
    {
        items.append(new QStandardItem(it->getLabel().c_str()));
    }
    sets_item->appendColumn(items);

    if (current_plot_qindex.isValid())
    {
        Plot* current_plot = current_plot_qindex.data(TreeView::PointerRole).value<Plot*>();
        auto dataset_ptr = current_plot->addDataSet(std::move(dataset));
        item->data(TreeView::PointerRole) = QVariant::fromValue(dataset_ptr);
    }

}

void MainWindow::addChart(bool set_current)
{
    auto root = model.invisibleRootItem();
    auto item = ac::TreeView::createItem(ac::TreeView::Item::Chart);
    root->appendRow(item);
    auto index = item->index();
    index.data(TreeView::PointerRole) =
            QVariant::fromValue(std::unique_ptr<Plot>(new Plot(this->getPlotWidget())));

    if (set_current)
        current_plot_qindex = index;
}

void MainWindow::configPlot()
{
    QCustomPlot* plot = getPlotWidget();

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
    if (current_plot_qindex.isValid())
    {
        Plot* cp = current_plot_qindex.data(TreeView::PointerRole).value<Plot*>();
        cp->addFunction(func);
    }
}
