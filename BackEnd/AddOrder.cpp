#include "AddOrder.h"
#include "Algorithms/SugestedPriceForClient.h"
#include "Common.h"
#include "DataBase/DataBaseApi.h"
#include "ui_mainwindow.h"
#include <QComboBox>
#include <QDate>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QPair>
#include <QPushButton>
#include <QString>
#include <QTableWidget>
#include <QTextEdit>

namespace BackEnd {

AddOrder::AddOrder(Ui::MainWindow* ui, DataBaseApi::DataBaseApi& databaseApi)
    : mAddOrderButton(*ui->addOrderTabAddOrderButton)
    , mCalculateButton(*ui->addOrderTabCalculateButton)
    , mAmount(*ui->addOrderTabAmount)
    , mCity(*ui->addOrderTabCity)
    , mIncome(*ui->addOrderTabIncome)
    , mPricePerLiter(*ui->addOrderTabPricePerLiter)
    , mNumber(*ui->addOrderTabNumber)
    , mOrdererName(*ui->addOrderTabOrdererName)
    , mStreet(*ui->addOrderTabStreet)
    , mTotalPrice(*ui->addOrderTabTotalPrice)
    , mDistance(*ui->addOrderTabDistance)
    , mTravelTime(*ui->addOrderTabTravelTime)
    , mFuelTypeBox(*ui->addOrderTabFuelTypeBox)
    , mDate(*ui->addOrderTabDate)
    , mDatabaseApi(databaseApi)
    , mChoosenFuelType(Common::FuelType::ERR)
{
    //
    mPricePerLiter.setText(QString("---"));
    mDistance.setText(QString("---"));
    mTravelTime.setText(QString("---"));
    mTotalPrice.setText(QString("---"));

    connect(ui->addOrderTabAddOrderButton, SIGNAL(pressed()), this, SLOT(addOrderPressed()));
    connect(ui->addOrderTabCalculateButton, SIGNAL(pressed()), this, SLOT(calculatePressed()));
    connect(ui->addOrderTabFuelTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(fuelTypeChanged(int)));
    connect(ui->addOrderTabDate, SIGNAL(selectionChanged()), this, SLOT(chosenDateChanged()));
}

void AddOrder::chosenDateChanged()
{
    mSelectedDate = mDate.selectedDate();

    qDebug("chosenDateChanged pressed %d:%d:%d", mSelectedDate.day(), mSelectedDate.month(), mSelectedDate.year());
}

QDate AddOrder::getDate()
{
    if (mSelectedDate.isNull())
        throw QString("Data realizacji zamowienia nie zostala dodana!");
    return mSelectedDate;
}

QString AddOrder::getOrdererName()
{
    QString tmp = mOrdererName.text();

    if (not tmp.size())
        throw QString("Pole nazwa obiorcy nie moze byc puste!");

    return tmp;
}

uint AddOrder::getAmount()
{
    QString tmp = mAmount.text();

    if (not tmp.size())
        throw QString("Pole ilość paliwa nie moze byc puste!");

    bool result = true;
    uint amount = tmp.toUInt(&result);

    if (result == false)
        throw QString("Blędna ilość paliwa!");

    return amount;
}

QString AddOrder::getCity()
{
    QString tmp = mCity.text();

    if (not tmp.size())
        throw QString("Pole miasto nie moze byc puste!");

    return tmp;
}

uint AddOrder::getIncome()
{
    QString tmp = mIncome.text();

    if (not tmp.size())
        throw QString("Pole planowany zysk nie moze byc puste!");

    bool result = true;
    uint income = tmp.toUInt(&result);

    if (result == false)
        throw QString("Błędna wartość w polu planowany zysk!");

    return income;
}

QString AddOrder::getNumber()
{
    QString tmp = mNumber.text();

    if (not tmp.size())
        throw QString("Pole numer posesji nie moze byc puste!");

    return tmp;
}

QString AddOrder::getStreet()
{
    QString tmp = mStreet.text();

    if (not tmp.size())
        throw QString("Pole ulica nie moze byc puste!");

    return tmp;
}

Common::FuelType AddOrder::getFuelType()
{
    if (mChoosenFuelType == Common::FuelType::ERR)
        throw QString("Nie wybrano typu paliwa!");

    return mChoosenFuelType;
}
void AddOrder::clearWindow()
{
    mOrdererName.clear();
    mStreet.clear();
    mNumber.clear();
    mCity.clear();
    mFuelTypeBox.setCurrentIndex(0);
    mAmount.clear();
    mIncome.clear();
    mPricePerLiter.setText(QString("---"));
    mDistance.setText(QString("---"));
    mTravelTime.setText(QString("---"));
    mTotalPrice.setText(QString("---"));
}

void AddOrder::calculatePressed()
{
    try
    {
        getOrdererName();
        getStreet();
        getNumber();
        getCity();
        getFuelType();
        getAmount();
        getIncome();

        Common::OrdersStruct tmp;

        tmp.amount            = getAmount();
        tmp.customer          = Common::CustomerStruct(getOrdererName(), getCity(), getStreet(), getNumber());
        tmp.date              = getDate();
        tmp.fuelType          = Common::getFuelTypeName(getFuelType());
        tmp.establishedProfit = getIncome();

        Algorithms::SugestedPriceForClient price(tmp);
        price.CalculateParameters();
        mPricePerLiter.setText(QString::number(price.GetOrderStruct().mPricePerLiter, 'f', 2));
        mDistance.setText(QString::number(price.GetOrderStruct().mDistance, 'f', 1));
        mTravelTime.setText(price.GetOrderStruct().mTravelTime.toString());
        mTotalPrice.setText(QString::number(price.GetOrderStruct().mTotalPrice));

        mAddOrderButton.setEnabled(true);
    }
    catch (QString& e)
    {
        QMessageBox msgBox;
        msgBox.setFont(QFont(QString("Arial"), 14));
        msgBox.setText(e);
        msgBox.exec();
    }

    qDebug("calculatePressed");
}

void AddOrder::addOrderPressed()
{
    QMessageBox msgBox;
    try
    {
        Common::OrdersStruct tmp;

        tmp.amount            = getAmount();
        tmp.customer          = Common::CustomerStruct(getOrdererName(), getCity(), getStreet(), getNumber());
        tmp.date              = getDate();
        tmp.establishedProfit = getIncome();
        tmp.fuelType          = Common::getFuelTypeName(getFuelType());

        Algorithms::SugestedPriceForClient price(tmp);
        price.CalculateParameters();

        tmp.totalPrice = price.GetOrderStruct().mTotalPrice;
        mDatabaseApi.addOrder(tmp);

        msgBox.setFont(QFont(QString("Arial"), 14));
        msgBox.setText(QString("Zamówienie dodane."));
        msgBox.exec();

        clearWindow();
    }
    catch (QString& e)
    {
        QMessageBox msgBox;
        msgBox.setFont(QFont(QString("Arial"), 14));
        msgBox.setText(e);
        msgBox.exec();
    }

    qDebug("addOrderPressed");
}

void AddOrder::fuelTypeChanged(int typeID)
{
    qDebug("FuelType changed to %d", typeID);
    switch (typeID)
    {
    case 0:
        mChoosenFuelType = Common::FuelType::ERR;
        break;
    case 1:
        mChoosenFuelType = Common::FuelType::ON;
        break;
    case 2:
        mChoosenFuelType = Common::FuelType::OO;
        break;
    default:
        mChoosenFuelType = Common::FuelType::ERR;
    }
}
} // namespace BackEnd
