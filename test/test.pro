TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

SOURCES +=  main.cpp \
            ../src/unit.cpp \
            ../src/model/*.cpp \
            ../src/model/treemodel/*.cpp \
    function_test.cpp

HEADERS +=  ../src/unit.h \
            ../src/model/*.h \
            ../src/model/treemodel/*.h

QT  += core gui charts
