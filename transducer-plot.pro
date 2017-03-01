#-------------------------------------------------
#
# Project created by QtCreator 2016-10-14T08:26:48
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = transducer-plot
TEMPLATE = app

CONFIG += c++14 console

QMAKE_CXXFLAGS += -std=c++14

SOURCES += \
    src/io/filereader.cpp \
    src/model/function.cpp \
    src/model/plot.cpp \
    src/model/set.inl \
    src/model/transducer.cpp \
    src/model/unit.cpp \
    src/view/broom.cpp \
    src/view/plotpresenter.cpp \
    src/view/transducerdelegate.cpp \
    src/viewmodel/treemodel/branchfilterproxymodel.cpp \
    src/viewmodel/treemodel/flattentreeproxymodel.cpp \
    src/viewmodel/treemodel/treeitem.cpp \
    src/viewmodel/treemodel/treeitemmodel.cpp \
    src/viewmodel/functionitem.cpp \
    src/viewmodel/plotitem.cpp \
    src/viewmodel/plotstoreitemmodel.cpp \
    src/viewmodel/setitem.cpp \
    src/viewmodel/transduceritem.cpp \
    src/viewmodel/transducersetproxymodel.cpp \
    src/functiondialog.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/viewmodel/treemodel/treeitemt.inl \
    src/view/unitaxis.cpp \
    src/viewmodel/transducertableproxymodel.cpp \
    src/io/tablemodelfilehandler.cpp \
    src/transducerdialog.cpp \
    src/view/plotitemdelegate.cpp \
    src/aboutdialog.cpp \
    src/screenshotform.cpp \
    src/pathvalidator.cpp \
    src/sizevalidator.cpp

HEADERS += \
    src/io/filereader.h \
    src/model/function.h \
    src/model/plot.h \
    src/model/set.h \
    src/model/transducer.h \
    src/model/unit.h \
    src/view/broom.h \
    src/view/plotpresenter.h \
    src/view/transducerdelegate.h \
    src/viewmodel/treemodel/branchfilterproxymodel.h \
    src/viewmodel/treemodel/flattentreeproxymodel.h \
    src/viewmodel/treemodel/treeitem.h \
    src/viewmodel/treemodel/treeitemmodel.h \
    src/viewmodel/functionitem.h \
    src/viewmodel/plotitem.h \
    src/viewmodel/plotstoreitemmodel.h \
    src/viewmodel/setitem.h \
    src/viewmodel/transduceritem.h \
    src/viewmodel/transducersetproxymodel.h \
    src/functiondialog.h \
    src/mainwindow.h \
    src/viewmodel/treemodel/treeitemt.h \
    src/view/unitaxis.h \
    src/viewmodel/transducertableproxymodel.h \
    src/io/tablemodelfilehandler.h \
    src/transducerdialog.h \
    src/view/plotitemdelegate.h \
    src/aboutdialog.h \
    src/screenshotform.h \
    src/pathvalidator.h \
    src/sizevalidator.h

FORMS    += src/mainwindow.ui \
    src/functiondialog.ui \
    src/transducerdialog.ui \
    src/aboutdialog.ui \
    src/screenshotform.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/release/
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/debug/
else:unix: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/

INCLUDEPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu

unix: LIBS += -lboost_system -lboost_filesystem
win32-g++: LIBS += -lboost_system-mt -lboost_filesystem-mt

DISTFILES +=

RESOURCES += \
    resources/resources.qrc
