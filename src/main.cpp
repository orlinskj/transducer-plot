#include "mainwindow.h"
#include <QApplication>

#include <QChart>

#include "ui_mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // a.installEventFilter(&w);

    return a.exec();
}
