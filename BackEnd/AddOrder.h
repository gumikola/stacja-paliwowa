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

    QString          getOrdererName();
    uint             getAmount();
    QString          getCity();
    uint             getIncome();
    QString          getNumber();
    QString          getStreet();
    void             clearWindow();
    QDate            getDate();
    Common::FuelType getFuelType();

public:
    AddOrder(DataBaseApi::DataBaseApi& databaseApi);
    void exec();

signals:

public slots:

    void calculatePressed();
    void addOrderPressed();
    void fuelTypeChanged(int);
    void chosenDateChanged();
};

} // namespace BackEnd

#endif // ADDORDER_H
