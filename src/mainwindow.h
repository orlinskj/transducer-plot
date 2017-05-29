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
#include "io/loader.h"
#include "model/plot.h"
#include "viewmodel/plotstoreitemmodel.h"
#include "view/plotpresenter.h"

#include "screenshotform.h"

using namespace std;
using namespace QtCharts;

namespace Ui {
    class MainWindow;
}

class TransducerDialog;
class AboutDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void resizeEvent(QResizeEvent* event);

    void add_plot();
    void remove_plot();
    void show_plot();
    void plot_view_menu(const QPoint &point);

    void add_function();
    void remove_function();

    void add_transducer();
    void remove_transducer();

    void show_transducer_dialog(int tab);
    void show_about_dialog();

private:
    void init_signals();
    void setup_view();
    // void seed();

public:
    Ui::MainWindow *ui_;
private:

    // Transducer model - stores all transducers
    TreeItemModel transducer_model_;

    //  stores plots and their functions
    PlotStoreItemModel plot_store_;

    // presents plot on QChartView
    PlotPresenter* plot_presenter_;

    // dialog window handlers
    TransducerDialog* transducer_dialog_;
    AboutDialog* about_dialog_;
    ScreenshotForm* screenshot_form_;

    QMenu plot_menu_;
};


#endif // MAINWINDOW_H
