TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle

SOURCES +=  main.cpp \
            ../src/model/*.cpp \
            ../src/viewmodel/treemodel/*.cpp \
            function_test.cpp \
            unit_test.cpp \
            ../src/error.cpp \
            treeitem_test.cpp

HEADERS +=  ../src/model/*.h \
            ../src/viewmodel/treemodel/*.h \
            ../src/error.cpp

QT  += core gui charts

QMAKE_CXXFLAGS += -std=c++14 -fext-numeric-literals
LIBS += -lboost_unit_test_framework
