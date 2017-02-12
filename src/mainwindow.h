#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QStandardItemModel>
#include <QChart>
#include <QChartView>
#include <QPushButton>
#include <QMenu>

#include "model/transducer.h"
#include "io/filereader.h"
#include "model/plot.h"
#include "model/plotstoreitemmodel.h"
#include "view/plotpresenter.h"

using namespace std;
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

    void add_transducer(Transducer *transducer);
    void add_function(const Function& func);

private slots:
    void slot_add_new_plot();
    void slot_remove_plot();
    void slot_add_function();
    void slot_remove_function();
    void slot_file_open();
    void slot_on_tree_view_context_menu(const QPoint &point);
    void slot_show_plot();
    void slot_update_plot();

private:
    void init_signals();
    void setup_view();
    void seed();

    void create_menus();

    Ui::MainWindow *ui_;

    // Transducer model - stores all transducers
    //QStandardItemModel transducer_model_;
    TreeItemModel transducer_model_;

    //  stores plots and their functions
    PlotStoreItemModel plot_store_;

    // presents plot on QChartView
    PlotPresenter* plot_presenter_;

    QMenu plot_menu_;
};


#endif // MAINWINDOW_H
