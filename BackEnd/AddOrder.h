#ifndef ADDORDER_H
#define ADDORDER_H

#include "Common.h"
#include "DataBase/DataBaseApi.h"
#include "ui_mainwindow.h"
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

namespace BackEnd {

class AddOrder : public QObject
{
    Q_OBJECT

    QPushButton&             mAddOrderButton;
    QPushButton&             mCalculateButton;
    QLineEdit&               mAmount;
    QLineEdit&               mCity;
    QLineEdit&               mIncome;
    QLineEdit&               mMargin;
    QLineEdit&               mNumber;
    QLineEdit&               mOrdererName;
    QLineEdit&               mStreet;
    QLineEdit&               mTotalPrice;
    QLineEdit&               mDistance;
    QLineEdit&               mTravelTime;
    QComboBox&               mFuelTypeBox;
    QCalendarWidget&         mDate;
    DataBaseApi::DataBaseApi mDatabaseApi;
    Common::FuelType         mChoosenFuelType;
    QDate                    mSelectedDate;

    QString getOrdererName();
    uint    getAmount();
    QString getCity();
    uint    getIncome();
    QString getNumber();
    QString getStreet();
    void    clearWindow();
    QDate   getDate();

public:
    AddOrder(Ui::MainWindow* ui, DataBaseApi::DataBaseApi& databaseApi);

signals:

public slots:

    void calculatePressed();
    void addOrderPressed();
    void fuelTypeChanged(int);
    void chosenDateChanged();
};

} // namespace BackEnd

#endif // ADDORDER_H
