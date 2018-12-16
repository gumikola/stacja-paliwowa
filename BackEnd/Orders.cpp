
#include "Orders.h"
#include "DataBase/DataBaseApi.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCalendarWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>

namespace BackEnd {

Orders::Orders(Ui::MainWindow* ui, DataBaseApi::DataBaseApi& dataBaseApi)
    : mOrders(*ui->ordersTabOrdersTable)
    , mWyswietlButton(*ui->ordersTabWyswietlButton)
    , mCalendar(*ui->ordersTabCalendar)
    , mAdditionalInformation(*ui->ordersTabAdditionalInfoText)
    , mDataBaseApi(dataBaseApi)
{
    connect(ui->ordersTabWyswietlButton, SIGNAL(pressed()), this, SLOT(wyswietlPressed()));
    connect(ui->ordersTabCalendar, SIGNAL(selectionChanged()), this, SLOT(chosenDateChanged()));

    printEmptyTable();
}

void Orders::printEmptyTable(void)
{
    int columnCnt = 0;

    mOrders.insertColumn(columnCnt);
    mOrders.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Nazwa"));

    mOrders.insertColumn(columnCnt);
    mOrders.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Miasto"));

    mOrders.insertColumn(columnCnt);
    mOrders.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Ulica"));

    mOrders.insertColumn(columnCnt);
    mOrders.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Numer"));

    mOrders.insertColumn(columnCnt);
    mOrders.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Ilosc"));

    mOrders.insertColumn(columnCnt);
    mOrders.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Cena sumaryczna"));

    mOrders.insertColumn(columnCnt);
    mOrders.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Przychód"));
}

void Orders::printOrders()
{
    QVector<Common::OrdersStruct> orders = mDataBaseApi.getOrdersByDate(mSelectedDate);
    int                           rowCnt = 0;
    mOrders.setRowCount(0);
    for (Common::OrdersStruct order : orders)
    {
        int columnCnt = 0;
        mOrders.insertRow(rowCnt);
        mOrders.setItem(rowCnt, columnCnt++, new QTableWidgetItem(order.customer.name));
        mOrders.setItem(rowCnt, columnCnt++, new QTableWidgetItem(order.customer.city));
        mOrders.setItem(rowCnt, columnCnt++, new QTableWidgetItem(order.customer.street));
        mOrders.setItem(rowCnt, columnCnt++, new QTableWidgetItem(order.customer.propertyNumber));
        mOrders.setItem(rowCnt, columnCnt++, new QTableWidgetItem(QString::number(order.amount)));
        mOrders.setItem(rowCnt, columnCnt++, new QTableWidgetItem(QString::number(order.totalPrice)));
        mOrders.setItem(rowCnt, columnCnt++, new QTableWidgetItem(QString::number(order.establishedProfit)));

        rowCnt++;
    }
}

void Orders::wyswietlPressed()
{
    printOrders();
    qDebug("wystiwetl preseses");
}

void Orders::chosenDateChanged()
{
    mSelectedDate = mCalendar.selectedDate();
    mDataBaseApi.getOrdersByDate(mSelectedDate);

    printOrders();
    qDebug("chosenDateChanged pressed %d:%d:%d", mSelectedDate.day(), mSelectedDate.month(), mSelectedDate.year());
}
} // namespace BackEnd
