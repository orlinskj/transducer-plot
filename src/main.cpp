#include "mainwindow.h"
#include <QApplication>

#include <qcustomplot.h>

using namespace ac;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;

    w.configPlot();

    w.show();

    return a.exec();
}
