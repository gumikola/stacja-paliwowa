
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
    int rowCnt    = 0;
    int columnCnt = 0;

    mOrders.insertRow(rowCnt);
    mOrders.insertColumn(columnCnt);

    mOrders.setItem(rowCnt, columnCnt, new QTableWidgetItem("pierwszy"));

    mOrders.insertColumn(++columnCnt);
    mOrders.setItem(rowCnt, columnCnt, new QTableWidgetItem("drugi"));
}

void Orders::wyswietlPressed()
{
    QVector<Common::OrdersStruct> orders = mDataBaseApi.getOrdersByDate(mSelectedDate);
    qDebug("wystiwetl preseses");
}

void Orders::chosenDateChanged()
{
    mSelectedDate = mCalendar.selectedDate();
    mDataBaseApi.getOrdersByDate(mSelectedDate);

    qDebug("chosenDateChanged pressed %d:%d:%d", mSelectedDate.day(), mSelectedDate.month(), mSelectedDate.year());
}
} // namespace BackEnd
