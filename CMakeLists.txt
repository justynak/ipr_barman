cmake_minimum_required(VERSION 3.16)
project(ipr_barman LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Sql)

add_executable(ipr_barman
    main.cpp
    mainwindow.cpp
    editwindow.cpp
    loginwindow.cpp
    manageproductwindow.cpp
    detailswindow.cpp
    ordermanager.cpp
    orderlist.cpp
    order.cpp
    orderdetails.cpp
    product.cpp
    productmanager.cpp
    categorylist.cpp
    customerscanner.cpp
    bartender.cpp
    databaseconnector.cpp
    loginscanner.cpp

    mainwindow.h
    editwindow.h
    loginwindow.h
    manageproductwindow.h
    detailswindow.h
    ordermanager.h
    orderlist.h
    order.h
    orderdetails.h
    product.h
    productmanager.h
    categorylist.h
    customerscanner.h
    bartender.h
    databaseconnector.h
    loginscanner.h

    mainwindow.ui
    editwindow.ui
    loginwindow.ui
    detailswindow.ui
    manageproductwindow.ui
)

target_link_libraries(ipr_barman PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql)
