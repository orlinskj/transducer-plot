#-------------------------------------------------
#
# Project created by QtCreator 2016-10-14T08:26:48
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = transducer-plot
TEMPLATE = app

CONFIG += c++11 console

QMAKE_CXXFLAGS += -std=c++11


SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/unit.cpp \
    src/model/function.cpp \
    src/model/set.inl \
    src/model/transducer.cpp \
    src/model/plot.cpp \
    src/io/filereader.cpp \
    src/view/transducerdelegate.cpp \
    src/view/plotitemsdelegate.cpp \
    src/view/plotpresenter.cpp \
    src/model/plotstoreitemmodel.cpp \
    src/model/plotstoreitem.cpp \
    src/model/treemodel/treeitem.inl \
    src/model/treemodel/treeitemmodel.cpp \
    src/model/treemodel/treenodeitem.cpp

HEADERS  += \
    src/mainwindow.h \
    src/unit.h \
    src/model/function.h \
    src/model/set.h \
    src/model/transducer.h \
    src/model/plot.h \
    src/io/filereader.h \
    src/view/transducerdelegate.h \
    src/view/plotitemsdelegate.h \
    src/view/plotpresenter.h \
    src/model/plotstoreitemmodel.h \
    src/model/plotstoreitem.h \
    src/model/treemodel/treeitem.h \
    src/model/treemodel/treeitemmodel.h \
    src/model/treemodel/treenodeitem.h

FORMS    += src/mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/release/ -lqcustomplot
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/debug/ -lqcustomplot
else:unix: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -lqcustomplot

INCLUDEPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu

unix|win32: LIBS += -lboost_system -lboost_filesystem

DISTFILES +=

RESOURCES += \
    resources/resources.qrc
