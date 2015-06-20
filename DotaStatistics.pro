#-------------------------------------------------
#
# Project created by QtCreator 2015-06-17T23:18:24
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DotaStatistics
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    utility.cpp \
    heroesrateandused.cpp

HEADERS  += mainwindow.h \
    utility.h \
    heroesrateandused.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    heroes.xml \
    items.xml