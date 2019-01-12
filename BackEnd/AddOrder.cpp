#include "AddOrder.h"
#include "AddClient.h"
#include "Algorithms/SugestedPriceForClient.h"
#include "ChooseCustomer.h"
#include "Common.h"
#include "DataBase/DataBaseApi.h"
#include <QDate>
#include <QDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QPair>
#include <QString>

namespace BackEnd {

AddOrder::AddOrder(DataBaseApi::DataBaseApi& databaseApi)
    : mUi(new Ui::AddOrder)
    , mDatabaseApi(databaseApi)
    , mChoosenFuelType(Common::FuelType::ERR)
{
    mUi->setupUi(&mDialog);

    connect(mUi->addOrderButton, SIGNAL(pressed()), this, SLOT(addOrderPressed()));
    connect(mUi->calculateButton, SIGNAL(pressed()), this, SLOT(calculatePressed()));
    connect(mUi->chooseClientFromDatabase, SIGNAL(pressed()), this, SLOT(chooseCustomerPressed()));
    connect(mUi->fuelTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(fuelTypeChanged(int)));
    connect(mUi->date, SIGNAL(selectionChanged()), this, SLOT(chosenDateChanged()));
    connect(mUi->addNewClient, SIGNAL(pressed()), this, SLOT(addNewCustomerPressed()));

    mDialog.open();
    mUi->pricePerLiter->setText(QString("---"));
    mUi->tabDistance->setText(QString("---"));
    mUi->travelTime->setText(QString("---"));
    mUi->totalPrice->setText(QString("---"));
    printDefaultClientsTable();
}

void AddOrder::exec()
{
    qDebug() << __PRETTY_FUNCTION__;

    mDialog.exec();
}

void AddOrder::chosenDateChanged()
{
    mSelectedDate = mUi->date->selectedDate();
    qDebug() << __PRETTY_FUNCTION__ << "Chosen date: " << mSelectedDate;
}

void AddOrder::chooseCustomerPressed()
{
    qDebug() << __PRETTY_FUNCTION__;

    ChooseCustomer window(mDatabaseApi);
    connect(&window, SIGNAL(clientChosed(const Common::CustomerStruct&)), this,
            SLOT(clientChosen(const Common::CustomerStruct&)));
    window.exec();
}

void AddOrder::clientChosen(const Common::CustomerStruct& client)
{
    qDebug() << __PRETTY_FUNCTION__;

    mChosenCustomer = client;

    int rowCnt = 0;
    mUi->choosenClientTable->setRowCount(rowCnt);
    int columnCnt = 0;
    mUi->choosenClientTable->insertRow(rowCnt);
    mUi->choosenClientTable->setVerticalHeaderItem(rowCnt, new QTableWidgetItem());

    mUi->choosenClientTable->setItem(rowCnt, columnCnt, new QTableWidgetItem(client.name));
    mUi->choosenClientTable->item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignHCenter);

    mUi->choosenClientTable->setItem(rowCnt, columnCnt, new QTableWidgetItem(client.city));
    mUi->choosenClientTable->item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignHCenter);

    mUi->choosenClientTable->setItem(rowCnt, columnCnt, new QTableWidgetItem(client.street));
    mUi->choosenClientTable->item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignHCenter);

    mUi->choosenClientTable->setItem(rowCnt, columnCnt,
                                     new QTableWidgetItem(client.propertyNumber));
    mUi->choosenClientTable->item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignHCenter);
}

void AddOrder::addNewCustomerPressed()
{
    qDebug() << __PRETTY_FUNCTION__;

    AddClient window(mDatabaseApi);
    connect(&window, SIGNAL(clientAdded(const Common::CustomerStruct&)), this,
            SLOT(clientChosen(const Common::CustomerStruct&)));
    window.exec();
}

QDate AddOrder::getDate()
{
    qDebug() << __PRETTY_FUNCTION__;

    if (mSelectedDate.isNull())
        throw QString("Data realizacji zamowienia nie zostala dodana!");
    return mSelectedDate;
}

uint AddOrder::getAmount()
{
    qDebug() << __PRETTY_FUNCTION__;

    QString tmp = mUi->amount->text();

    if (not tmp.size())
        throw QString("Pole ilość paliwa nie moze byc puste!");

    bool result = true;
    uint amount = tmp.toUInt(&result);

    if (result == false)
        throw QString("Blędna ilość paliwa!");

    return amount;
}

uint AddOrder::getIncome()
{
    qDebug() << __PRETTY_FUNCTION__;

    QString tmp = mUi->income->text();

    if (not tmp.size())
        throw QString("Pole planowany zysk nie moze byc puste!");

    bool result = true;
    uint income = tmp.toUInt(&result);

    if (result == false)
        throw QString("Błędna wartość w polu planowany zysk!");

    return income;
}

Common::FuelType AddOrder::getFuelType()
{
    qDebug() << __PRETTY_FUNCTION__;

    if (mChoosenFuelType == Common::FuelType::ERR)
        throw QString("Nie wybrano typu paliwa!");

    return mChoosenFuelType;
}

void AddOrder::printDefaultClientsTable()
{
    qDebug() << __PRETTY_FUNCTION__;

    mUi->choosenClientTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    mUi->choosenClientTable->setSelectionMode(QAbstractItemView::SingleSelection);
    mUi->choosenClientTable->setContextMenuPolicy(Qt::CustomContextMenu);

    int columnCnt = 0;
    mUi->choosenClientTable->setFont(QFont(QString("Arial"), 14));

    mUi->choosenClientTable->setColumnCount(columnCnt);
    mUi->choosenClientTable->setRowCount(0);

    mUi->choosenClientTable->insertColumn(columnCnt);
    mUi->choosenClientTable->setHorizontalHeaderItem(columnCnt++,
                                                     new QTableWidgetItem("Imie i nazwisko"));

    mUi->choosenClientTable->insertColumn(columnCnt);
    mUi->choosenClientTable->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Miasto"));

    mUi->choosenClientTable->insertColumn(columnCnt);
    mUi->choosenClientTable->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Ulica"));

    mUi->choosenClientTable->insertColumn(columnCnt);
    mUi->choosenClientTable->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Numer"));

    for (int i = 0; i < columnCnt; i++)
    {
        if (i == 0)
            mUi->choosenClientTable->setColumnWidth(i, mUi->choosenClientTable->width() / 3 - 16);
        else if (i == 3)
            mUi->choosenClientTable->setColumnWidth(i, 70);
        else
            mUi->choosenClientTable->setColumnWidth(
                i, (2 * mUi->choosenClientTable->width() / 3 - 70) / (columnCnt - 2));
    }
}
void AddOrder::clearWindow()
{
    qDebug() << __PRETTY_FUNCTION__;

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
    qDebug() << __PRETTY_FUNCTION__;

    try
    {
        getFuelType();
        getAmount();
        getIncome();

        Common::OrdersStruct tmp;

        tmp.amount            = getAmount();
        tmp.customer          = mChosenCustomer;
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
}

void AddOrder::addOrderPressed()
{
    qDebug() << __PRETTY_FUNCTION__;

    QMessageBox msgBox;
    try
    {
        Common::OrdersStruct tmp;

        tmp.amount            = getAmount();
        tmp.customer          = {};
        tmp.date              = getDate();
        tmp.establishedProfit = getIncome();
        tmp.fuelType          = getFuelType();

        Algorithms::SugestedPriceForClient price(tmp);
        price.CalculateParameters();

        tmp.totalPrice = price.GetOrderStruct().mTotalPrice;
        mDatabaseApi.addOrder(tmp);

        emit newOrderAdded(tmp);
        msgBox.setFont(QFont(QString("Arial"), 14));
        msgBox.setText(QString("Zamówienie dodane."));
        msgBox.exec();

        mDialog.close();
    }
    catch (QString& e)
    {
        QMessageBox msgBox;
        msgBox.setFont(QFont(QString("Arial"), 14));
        msgBox.setText(e);
        msgBox.exec();
    }
}

void AddOrder::fuelTypeChanged(int typeID)
{
    qDebug() << __PRETTY_FUNCTION__ << " Fuel type changed to:"
             << Common::getFuelTypeName(static_cast<Common::FuelType>(typeID));
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
