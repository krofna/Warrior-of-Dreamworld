#-------------------------------------------------
#
# Project created by QtCreator 2012-08-31T23:55:24
#
#-------------------------------------------------

QT       += core gui sql

TARGET = world_edit
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.hpp

FORMS    += MainWindow.ui

unix:!macx:!symbian: LIBS += -lboost_serialization
QMAKE_CXXFLAGS += -std=c++0x
