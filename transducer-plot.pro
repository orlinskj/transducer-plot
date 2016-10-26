#-------------------------------------------------
#
# Project created by QtCreator 2016-10-14T08:26:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = transducer-plot
TEMPLATE = app

CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11


SOURCES += \
    src/dataset.cpp \
    src/filereader.cpp \
    src/function.cpp \
    src/functionqt.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/plot.cpp \
    src/plotqt.cpp

HEADERS  += \
    mainwindow.h \
    ../../../../usr/include/qcustomplot.h \
    src/dataset.h \
    src/filereader.h \
    src/function.h \
    src/functionqt.h \
    src/mainwindow.h \
    src/plot.h \
    src/plotqt.h \
    src/series.h

FORMS    += src/mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/release/ -lqcustomplot
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/debug/ -lqcustomplot
else:unix: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -lqcustomplot

INCLUDEPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu

unix|win32: LIBS += -lboost_system -lboost_filesystem

DISTFILES +=
