#include "Orders.h"
#include "AddOrder.h"
#include "Algorithms/CalculateOrder.h"
#include "DataBase/DataBaseApi.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCalendarWidget>
#include <QMessageBox>

namespace BackEnd {

Orders::Orders(Ui::MainWindow* ui, DataBaseApi::DataBaseApi& dataBaseApi)
    : mOrders(*ui->ordersTabOrdersTable)
    , mWyswietlButton(*ui->ordersTabWyswietlButton)
    , mCalendar(*ui->ordersTabCalendar)
    , mDataBaseApi(dataBaseApi)
    , mAddOrderButton(*ui->ordersTabAddOrdetButton)
{
    connect(ui->ordersTabWyswietlButton, SIGNAL(pressed()), this, SLOT(wyswietlPressed()));
    connect(ui->ordersTabAddOrdetButton, SIGNAL(pressed()), this, SLOT(addOrderPressed()));
    connect(ui->ordersTabCalendar, SIGNAL(selectionChanged()), this, SLOT(chosenDateChanged()));
    connect(ui->ordersTabOrdersTable, SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(displayMenu(QPoint)));

    setWidthOfOrdersTableColumns(printEmptyOrdersTable());
}

int Orders::printEmptyOrdersTable()
{
    qDebug() << __PRETTY_FUNCTION__;

    int columnCnt = 0;

    mOrders.setColumnCount(columnCnt);

    mOrders.setSelectionBehavior(QAbstractItemView::SelectRows);
    mOrders.setSelectionMode(QAbstractItemView::SingleSelection);
    mOrders.setContextMenuPolicy(Qt::CustomContextMenu);

    mOrders.insertColumn(columnCnt);
    mOrders.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Nazwa"));

    mOrders.insertColumn(columnCnt);
    mOrders.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Miasto"));

    mOrders.insertColumn(columnCnt);
    mOrders.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Ulica"));

    mOrders.insertColumn(columnCnt);
    mOrders.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Numer"));

    mOrders.insertColumn(columnCnt);
    mOrders.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Typ\npaliwa"));

    mOrders.insertColumn(columnCnt);
    mOrders.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Ilość"));

    mOrders.insertColumn(columnCnt);
    mOrders.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Cena\nsumaryczna"));

    mOrders.insertColumn(columnCnt);
    mOrders.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Przychód"));

    return columnCnt;
}

int Orders::addCarColumnIntoOrdersTable(int columnCnt)
{
    qDebug() << __PRETTY_FUNCTION__;

    mOrders.insertColumn(columnCnt);
    mOrders.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Numer\ncysterny"));

    return columnCnt;
}

void Orders::setWidthOfOrdersTableColumns(int columnCnt)
{
    qDebug() << __PRETTY_FUNCTION__;

    for (int i = 0; i < columnCnt; i++)
    {
        if (i == 0)
            mOrders.setColumnWidth(i, mOrders.width() / 4 - 24);
        else if (i == 1 or i == 2)
            mOrders.setColumnWidth(i, mOrders.width() / 8);
        else
            mOrders.setColumnWidth(i, (2 * mOrders.width() / 4) / (columnCnt - 3));
    }
}

void Orders::printOrders()
{
    qDebug() << __PRETTY_FUNCTION__;

    setWidthOfOrdersTableColumns(printEmptyOrdersTable());

    if (mSelectedDate.isNull())
    {
        qDebug() << __PRETTY_FUNCTION__ << " Date wasn't selected!";
        return;
    }

    QVector<Common::OrdersStruct> orders = mDataBaseApi.getOrdersByDate(mSelectedDate);
    int                           rowCnt = 0;
    mOrders.setRowCount(0);
    for (Common::OrdersStruct order : orders)
    {
        int columnCnt = 0;
        mOrders.insertRow(rowCnt);
        mOrders.setItem(rowCnt, columnCnt, new QTableWidgetItem(order.customer.name));
        mOrders.item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignHCenter);

        mOrders.setItem(rowCnt, columnCnt, new QTableWidgetItem(order.customer.city));
        mOrders.item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignHCenter);

        mOrders.setItem(rowCnt, columnCnt, new QTableWidgetItem(order.customer.street));
        mOrders.item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignHCenter);

        mOrders.setItem(rowCnt, columnCnt, new QTableWidgetItem(order.customer.propertyNumber));
        mOrders.item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignHCenter);

        mOrders.setItem(rowCnt, columnCnt,
                        new QTableWidgetItem(Common::getFuelTypeName(order.fuelType)));
        mOrders.item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignHCenter);

        mOrders.setItem(rowCnt, columnCnt, new QTableWidgetItem(QString::number(order.amount)));
        mOrders.item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignHCenter);

        mOrders.setItem(rowCnt, columnCnt, new QTableWidgetItem(QString::number(order.totalPrice)));
        mOrders.item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignHCenter);

        mOrders.setItem(rowCnt, columnCnt,
                        new QTableWidgetItem(QString::number(order.establishedProfit)));
        mOrders.item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignHCenter);

        rowCnt++;
    }
}

void Orders::wyswietlPressed()
{
    qDebug() << __PRETTY_FUNCTION__;

    printCalculatedOrder();
}

void Orders::chosenDateChanged()
{
    mSelectedDate = mCalendar.selectedDate();

    qDebug() << __PRETTY_FUNCTION__ << "Selected date: " << mSelectedDate;

    mDataBaseApi.getOrdersByDate(mSelectedDate);

    printOrders();
}

void Orders::printCalculatedOrder()
{
    qDebug() << __PRETTY_FUNCTION__;
    QVector<Common::OrdersStruct>      databaseOrders = mDataBaseApi.getOrdersByDate(mSelectedDate);
    Algorithms::CalculateOrder         test(mDataBaseApi, databaseOrders.toList());
    QList<QList<Common::OrdersStruct>> calculated = test.getOrders();

    int rowCnt = 0;
    addCarColumnIntoOrdersTable(8);
    mOrders.setRowCount(0);
    for (int i = 0; i < calculated.size(); i++)
    {
        for (Common::OrdersStruct order : calculated[i])
        {
            int columnCnt = 0;
            mOrders.insertRow(rowCnt);
            mOrders.setItem(rowCnt, columnCnt++, new QTableWidgetItem(order.customer.name));
            mOrders.setItem(rowCnt, columnCnt++, new QTableWidgetItem(order.customer.city));
            mOrders.setItem(rowCnt, columnCnt++, new QTableWidgetItem(order.customer.street));
            mOrders.setItem(rowCnt, columnCnt++,
                            new QTableWidgetItem(order.customer.propertyNumber));
            mOrders.setItem(rowCnt, columnCnt++,
                            new QTableWidgetItem(QString::number(order.amount)));
            mOrders.setItem(rowCnt, columnCnt++,
                            new QTableWidgetItem(QString::number(order.totalPrice)));
            mOrders.setItem(rowCnt, columnCnt++,
                            new QTableWidgetItem(QString::number(order.establishedProfit)));

            mOrders.setItem(rowCnt, columnCnt++,
                            new QTableWidgetItem(QString::number(i + 1))); // numer cysterny
                                                                           //            od 1

            rowCnt++;
        }
    }
}

void Orders::addOrderPressed()
{
    qDebug() << __PRETTY_FUNCTION__;

    AddOrder window(mDataBaseApi);
    connect(&window, SIGNAL(newOrderAdded(const Common::OrdersStruct&)), this,
            SLOT(newOrderAdded(const Common::OrdersStruct&)));
    window.exec();
}

void Orders::displayMenu(QPoint pos)
{
    qDebug() << __PRETTY_FUNCTION__;

    QMenu menu(&(mOrders));

    QAction* addNewClient = menu.addAction("Dodaj nowe zamowienie");
    QAction* remove       = menu.addAction("Usuń");

    connect(addNewClient, SIGNAL(triggered()), this, SLOT(addOrderPressed()));
    connect(remove, SIGNAL(triggered()), this, SLOT(removeOrderPressed()));

    menu.exec(mOrders.viewport()->mapToGlobal(pos));
}

void Orders::removeOrderPressed()
{
    int row = mOrders.currentRow();

    qDebug() << __PRETTY_FUNCTION__ << " row=" << row;

    if (row >= 0)
    {
        Common::OrdersStruct tmp;
        tmp.customer.name           = mOrders.item(row, 0)->text();
        tmp.customer.city           = mOrders.item(row, 1)->text();
        tmp.customer.street         = mOrders.item(row, 2)->text();
        tmp.customer.propertyNumber = mOrders.item(row, 3)->text();
        tmp.fuelType                = Common::getFuelTypeEnum(mOrders.item(row, 4)->text());
        tmp.amount                  = mOrders.item(row, 5)->text().toUInt();
        tmp.totalPrice              = mOrders.item(row, 6)->text().toDouble();
        tmp.establishedProfit       = mOrders.item(row, 7)->text().toUInt();

        mDataBaseApi.removeOrder(tmp);
    }
}

void Orders::newOrderAdded(const Common::OrdersStruct& order)
{
    qDebug() << __PRETTY_FUNCTION__;
    if (order.date == mSelectedDate)
    {
        printOrders();
    }
}

} // namespace BackEnd
