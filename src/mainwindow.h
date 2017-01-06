#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QStandardItemModel>
#include <QChart>
#include <QChartView>

#include "model/transducer.h"
#include "io/filereader.h"
#include "model/plot.h"

using namespace std;
using namespace ac;
using namespace QtCharts;

namespace Ui {

    class MainWindow;

}

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void add_chart();
    void add_transducer(std::shared_ptr<Transducer> transducer);
    void add_function(const Function& func);

public slots:
    void file_open();
    void on_tree_view_context_menu(const QPoint &point);
    void test_slot();

private:
    void init_signals();
    void setup_view();

    Ui::MainWindow *ui_;

    // Transducer model - stores all transducers
    QStandardItemModel transducer_model_;

    // Plot model - stores plots and their functions
    QStandardItemModel plot_model_;

    QChartView charView;
};


#endif // MAINWINDOW_H
