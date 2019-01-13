#ifndef ADDORDER_H
#define ADDORDER_H

#include "Common.h"
#include "ui_add_order.h"
#include <QCalendarWidget>
#include <QComboBox>
#include <QDate>
#include <QLineEdit>
#include <QMainWindow>
#include <QObject>
#include <QPushButton>
#include <QString>
#include <QTableWidget>
#include <QTextEdit>

namespace DataBaseApi {
class DataBaseApi;
} // namespace DataBaseApi

namespace BackEnd {

class AddOrder : public QObject
{
    Q_OBJECT

    Ui::AddOrder*             mUi;
    DataBaseApi::DataBaseApi& mDatabaseApi;
    Common::FuelType          mChoosenFuelType;
    QDate                     mSelectedDate;
    QDialog                   mDialog;
    Common::CustomerStruct    mChosenCustomer;

    uint             getAmount();
    uint             getIncome();
    void             clearWindow();
    QDate            getDate();
    Common::FuelType getFuelType();
    void             printDefaultClientsTable();

public:
    AddOrder(DataBaseApi::DataBaseApi& databaseApi);
    void exec();

signals:
    void newOrderAdded(const Common::OrdersStruct& order);

public slots:

    void calculatePressed();
    void addOrderPressed();
    void fuelTypeChanged(int);
    void chosenDateChanged();
    void chooseCustomerPressed();
    void clientChosen(const Common::CustomerStruct& client);
    void addNewCustomerPressed();
};

} // namespace BackEnd

#endif // ADDORDER_H
