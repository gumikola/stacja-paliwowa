#-------------------------------------------------
#
# Project created by QtCreator 2018-11-17T16:23:54
#
#-------------------------------------------------

QT       += core gui sql network svg xml charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
# trzeba dodać do instalacji qt w Tools/mingw530_32/opt/bin dllki ssleay32.dll i libleay32.dll
TARGET = SystemZarzadzaniaStacjaPaliwowa
TEMPLATE = app
CONFIG += openssl-linked
unix: PKGCONFIG += openssl

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    BackEnd/mainwindow.cpp \
    BackEnd/Orders.cpp \
    DataBase/DataBaseApi.cpp \
    Algorithms/ClientSuggestion.cpp \
    Algorithms/GetFuelPriceFromWeb.cpp \ 
    Algorithms/SugestedPriceforClient.cpp\
    Algorithms/GoogleApi.cpp \
    BackEnd/AddOrder.cpp \
    Common.cpp \
    BackEnd/main.cpp \
    Algorithms/CalculateRoute.cpp \
    BackEnd/UpdateTanksFillLevel.cpp \
    Algorithms/CalculateOrder.cpp \
    BackEnd/FuelPriceChart.cpp \
    BackEnd/FuelTanks.cpp \
    BackEnd/ClientsTab.cpp \
    AddClient.cpp \
    BackEnd/EditClient.cpp

HEADERS += \
    BackEnd/mainwindow.h \
    BackEnd/Orders.h \
    DataBase/DataBaseApi.h \
    Algorithms/ClientSuggestion.h \
    Algorithms/GetFuelPriceFromWeb.h \
    Algorithms/SugestedPriceForClient.h \
    Algorithms/GoogleApi.h \
    BackEnd/AddOrder.h \
    Common.h \
    Algorithms/CalculateRoute.h \
    BackEnd/UpdateTanksFillLevel.h \
    BackEnd/FuelPriceChart.h \
    Algorithms/CalculateOrder.h \
    BackEnd/FuelTanks.h \
    BackEnd/ClientsTab.h \
    AddClient.h \
    BackEnd/EditClient.h


FORMS += \
    mainwindow.ui \
    add_order.ui \
    edit_tank_fill_lvl.ui \
    add_client.ui \
    edit_client.ui
