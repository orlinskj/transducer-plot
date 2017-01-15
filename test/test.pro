TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

SOURCES +=  main.cpp \
            ../src/model/*.cpp

HEADERS +=  ../src/model/*.h

QT  += core gui charts
