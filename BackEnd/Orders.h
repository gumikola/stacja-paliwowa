#ifndef ORDERS_H
#define ORDERS_H

#include "DataBase/DataBaseApi.h"
#include <QCalendarWidget>
#include <QObject>
#include <ui_mainwindow.h>

#include "ui_mainwindow.h"

namespace BackEnd {

class Orders : QObject
{
    Q_OBJECT
    QTableWidget&            mOrders;
    QPushButton&             mWyswietlButton;
    QCalendarWidget&         mCalendar;
    DataBaseApi::DataBaseApi mDataBaseApi;
    QPushButton&             mAddOrderButton;

    QDate mSelectedDate;

    void printOrders();
    void printCalculatedOrder();

public:
    Orders(Ui::MainWindow* ui, DataBaseApi::DataBaseApi& dataBaseApi);

    int  printEmptyOrdersTable(void);
    int  addCarColumnIntoOrdersTable(int columnCnt);
    void setWidthOfOrdersTableColumns(int columnCnt);
    void fillOrderOfOrders(const QList<Common::OrdersStruct>& orders, int& rowCnt, int carId);

private slots:
    void wyswietlPressed();
    void chosenDateChanged();
    void addOrderPressed();
    void displayMenu(QPoint pos);
    void removeOrderPressed();
    void newOrderAdded(const Common::OrdersStruct& order);
};

} // namespace BackEnd

#endif // ORDERS_H
