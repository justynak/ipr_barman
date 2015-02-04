#-------------------------------------------------
#
# Project created by QtCreator 2015-01-28T19:48:11
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ipr_barman
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    editwindow.cpp \
    loginwindow.cpp \
    manageproductwindow.cpp \
    detailswindow.cpp \
    ordermanager.cpp \
    orderlist.cpp \
    order.cpp \
    orderdetails.cpp \
    product.cpp \
    productmanager.cpp \
    categorylist.cpp \
    customerscanner.cpp \
    bartender.cpp \
    databaseconnector.cpp \
    loginscanner.cpp

HEADERS  += mainwindow.h \
    editwindow.h \
    loginwindow.h \
    manageproductwindow.h \
    detailswindow.h \
    ordermanager.h \
    orderlist.h \
    order.h \
    orderdetails.h \
    product.h \
    productmanager.h \
    categorylist.h \
    customerscanner.h \
    bartender.h \
    databaseconnector.h \
    loginscanner.h

FORMS    += mainwindow.ui \
    editwindow.ui \
    loginwindow.ui \
    detailswindow.ui \
    manageproductwindow.ui \
    dialog.ui
