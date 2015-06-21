#-------------------------------------------------
#
# Project created by QtCreator 2015-06-17T23:18:24
#
#-------------------------------------------------

QT       += core gui network xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DOTA2Statistics
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    utility.cpp \
    heroitems.cpp \
    herolist.cpp \
    dataconfig.cpp \
    datamanager.cpp \
    heroesusedandrate.cpp \
    heroitemsmanager.cpp \
    heroesusedandratemanager.cpp

HEADERS  += mainwindow.h \
    utility.h \
    heroitems.h \
    herolist.h \
    dataconfig.h \
    datamanager.h \
    Singleton.h \
    heroesusedandrate.h \
    heroitemsmanager.h \
    heroesusedandratemanager.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    heroes.xml \
    items.xml \
    dotastatistics.rc

RC_FILE = dotastatistics.rc
