TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

SOURCES +=  main.cpp \
            ../src/model/*.cpp \
            ../src/viewmodel/treemodel/*.cpp \
    function_test.cpp \
    unit_test.cpp

HEADERS +=  ../src/model/*.h \
            ../src/viewmodel/treemodel/*.h

QT  += core gui charts

QMAKE_CXXFLAGS += -std=c++14
LIBS += -lboost_unit_test_framework
