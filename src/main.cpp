#include "mainwindow.h"
#include <QApplication>

#include <QMessageBox>

#include "ui_mainwindow.h"
#include "error.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    bool running = true;
    int code = 0;
    while(running){
        try {
            code = app.exec();
            running = false;
        }
        catch(const Error& e){
            if (e.type() == Error::Type::Info)
                QMessageBox::information(&w,QObject::tr("Informacja"),e.what());
            else
                QMessageBox::critical(&w,QObject::tr("Błąd"),e.what());
        }
    }

    return code;
}
