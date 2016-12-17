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
    src/filereader.cpp \
    src/function.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/plot.cpp \
    src/treeview.cpp \
    src/unit.cpp \
    src/transducer.cpp \
    src/plotqt.cpp \
    src/set.inl

HEADERS  += \
    ../../../../usr/include/qcustomplot.h \
    src/mainwindow.h \
    src/plot.h \
    src/treeview.h \
    src/set.h \
    src/unit.h \
    src/transducer.h \
    src/plotqt.h

FORMS    += src/mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/release/ -lqcustomplot
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/debug/ -lqcustomplot
else:unix: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -lqcustomplot

INCLUDEPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu

unix|win32: LIBS += -lboost_system -lboost_filesystem

DISTFILES +=

RESOURCES += \
    resources/icons.qrc
