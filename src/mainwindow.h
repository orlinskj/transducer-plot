#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <qcustomplot.h>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <utility>

#include "dataset.h"
#include "filereader.h"
#include "plot.h"
#include "functionqt.h"

using namespace std;
using namespace ac;

namespace Ui {

    class MainWindow;

}

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QCustomPlot* getPlotHandle();
    void configPlot();
    void addDataSet(DataSet&& dataset);
    void addFunction(const Function& func);

public slots:
    void fileOpen();
    void onTreeViewContextMenu(const QPoint &point);
    void testSlot();

private:
    Ui::MainWindow *ui;

    // Current plot
    QPersistentModelIndex current_plot_qindex;

    // All plots
    vector<unique_ptr<PlotQt> > plots;
    // Current plot pointer
    PlotQt* current_plot;

    // Characteristics model
    QStandardItemModel model;
};


#endif // MAINWINDOW_H
