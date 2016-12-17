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

#include "transducer.h"
#include "filereader.h"
#include "plot.h"
#include "treeview.h"

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

    QCustomPlot* getPlotWidget();
    void configPlot();
    void addChart(bool set_current);
    void addDataSet(DataSet&& dataset);
    void addFunction(const Function& func);

public slots:
    void fileOpen();
    void onTreeViewContextMenu(const QPoint &point);
    void testSlot();

private:
    void initSignals();

    Ui::MainWindow *ui;

    // Current plot
    QPersistentModelIndex current_plot_qindex;

    // Characteristics model
    QStandardItemModel model;
};


#endif // MAINWINDOW_H
