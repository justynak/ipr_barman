cmake_minimum_required(VERSION 3.16)
project(ipr_barman LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Sql Test)

add_library(barman_core STATIC
    ordermanager.cpp
    draftorder.cpp
    businessday.cpp
    productmanager.cpp
    categorylist.cpp
    customerscanner.cpp
    bartender.cpp
    mysqlbarrepository.cpp
    loginscanner.cpp

    barrepository.h
    cardscanner.h
    ordermanager.h
    draftorder.h
    businessday.h
    money.h
    employee.h
    customer.h
    product.h
    productmanager.h
    categorylist.h
    customerscanner.h
    bartender.h
    mysqlbarrepository.h
    loginscanner.h
)

target_include_directories(barman_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(barman_core PUBLIC Qt5::Core Qt5::Sql)

add_executable(ipr_barman
    main.cpp
    mainwindow.cpp
    editwindow.cpp
    loginwindow.cpp
    manageproductwindow.cpp
    detailswindow.cpp

    mainwindow.h
    editwindow.h
    loginwindow.h
    manageproductwindow.h
    detailswindow.h

    mainwindow.ui
    editwindow.ui
    loginwindow.ui
    detailswindow.ui
    manageproductwindow.ui
)

target_link_libraries(ipr_barman PRIVATE barman_core Qt5::Gui Qt5::Widgets)

enable_testing()
add_subdirectory(tests)
