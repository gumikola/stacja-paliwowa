#include "AddOrder.h"
#include "Algorithms/SugestedPriceForClient.h"
#include "Common.h"
#include "DataBase/DataBaseApi.h"
#include <QComboBox>
#include <QDate>
#include <QDialog>
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

AddOrder::AddOrder(DataBaseApi::DataBaseApi& databaseApi)
    : mUi(new Ui::AddOrder)
    , mDatabaseApi(databaseApi)
    , mChoosenFuelType(Common::FuelType::ERR)
{

    mUi->setupUi(&mDialog);

    connect(mUi->addOrderButton, SIGNAL(pressed()), this, SLOT(addOrderPressed()));
    connect(mUi->calculateButton, SIGNAL(pressed()), this, SLOT(calculatePressed()));
    connect(mUi->fuelTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(fuelTypeChanged(int)));
    connect(mUi->date, SIGNAL(selectionChanged()), this, SLOT(chosenDateChanged()));

    mDialog.open();
    mUi->pricePerLiter->setText(QString("---"));
    mUi->tabDistance->setText(QString("---"));
    mUi->travelTime->setText(QString("---"));
    mUi->totalPrice->setText(QString("---"));
}

void AddOrder::exec()
{
    mDialog.exec();
}

void AddOrder::chosenDateChanged()
{
    mSelectedDate = mUi->date->selectedDate();

    qDebug("chosenDateChanged pressed %d:%d:%d", mSelectedDate.day(), mSelectedDate.month(),
           mSelectedDate.year());
}

QDate AddOrder::getDate()
{
    if (mSelectedDate.isNull())
        throw QString("Data realizacji zamowienia nie zostala dodana!");
    return mSelectedDate;
}

QString AddOrder::getOrdererName()
{
    QString tmp = mUi->ordererName->text();

    if (not tmp.size())
        throw QString("Pole nazwa obiorcy nie moze byc puste!");

    return tmp;
}

uint AddOrder::getAmount()
{
    QString tmp = mUi->amount->text();

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
    QString tmp = mUi->city->text();

    if (not tmp.size())
        throw QString("Pole miasto nie moze byc puste!");

    return tmp;
}

uint AddOrder::getIncome()
{
    QString tmp = mUi->income->text();

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
    QString tmp = mUi->number->text();

    if (not tmp.size())
        throw QString("Pole numer posesji nie moze byc puste!");

    return tmp;
}

QString AddOrder::getStreet()
{
    QString tmp = mUi->street->text();

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
    mUi->ordererName->clear();
    mUi->street->clear();
    mUi->number->clear();
    mUi->city->clear();
    mUi->fuelTypeBox->setCurrentIndex(0);
    mUi->amount->clear();
    mUi->income->clear();
    mUi->pricePerLiter->setText(QString("---"));
    mUi->tabDistance->setText(QString("---"));
    mUi->travelTime->setText(QString("---"));
    mUi->totalPrice->setText(QString("---"));
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

        tmp.amount = getAmount();
        tmp.customer =
            Common::CustomerStruct(getOrdererName(), getCity(), getStreet(), getNumber());
        tmp.date              = getDate();
        tmp.fuelType          = getFuelType();
        tmp.establishedProfit = getIncome();

        Algorithms::SugestedPriceForClient price(tmp);
        price.CalculateParameters();
        mUi->pricePerLiter->setText(QString::number(price.GetOrderStruct().mPricePerLiter, 'f', 2));
        mUi->tabDistance->setText(QString::number(price.GetOrderStruct().mDistance, 'f', 1));
        mUi->travelTime->setText(price.GetOrderStruct().mTravelTime.toString());
        mUi->totalPrice->setText(QString::number(price.GetOrderStruct().mTotalPrice));

        mUi->addOrderButton->setEnabled(true);
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

        tmp.amount = getAmount();
        tmp.customer =
            Common::CustomerStruct(getOrdererName(), getCity(), getStreet(), getNumber());
        tmp.date              = getDate();
        tmp.establishedProfit = getIncome();
        tmp.fuelType          = getFuelType();

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
