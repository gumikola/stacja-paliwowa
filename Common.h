#ifndef COMMON_H
#define COMMON_H
#include <QDate>
#include <QString>
#include <QTime>

namespace Common {

enum class FuelType
{
    ON,
    OO,
    PB95,
    PB98,
    ERR
};

enum class FuelTankType
{
    ON,
    ON1,
    ON2,
    ON3,
    OO,
    PB95,
    PB98,
    ERR
};

static QString URL = "https://www.orlen.pl/PL/DlaBiznesu/HurtoweCenyPaliw/Strony/default.aspx";
QString        getFuelTypeName(const FuelType& type);
FuelTankType   getFuelTankEnum(QString type);
QString        getFuelTankTypeName(const FuelTankType& type);

struct OrderParametersOutput
{
    QTime  mTravelTime;
    uint   mTotalPrice;
    double mPricePerLiter;
    double mDistance;
    uint   mIncome;

    OrderParametersOutput() {}
    OrderParametersOutput(QTime travelTime, uint totalPrice, double margin, double distance, uint income)
        : mTravelTime(travelTime)
        , mTotalPrice(totalPrice)
        , mPricePerLiter(margin)
        , mDistance(distance)
        , mIncome(income)
    {
    }
};

struct CustomerStruct
{
    QString name;
    QString city;
    QString street;
    QString propertyNumber;

    CustomerStruct(QString name = nullptr, QString city = nullptr, QString street = nullptr,
                   QString propertyNumber = nullptr)
        : name(name)
        , city(city)
        , street(street)
        , propertyNumber(propertyNumber)
    {
    }
};

struct OrdersStruct
{
    uint           amount;
    QDate          date;
    double         totalPrice;
    CustomerStruct customer;
    QString        fuelType;
    uint           establishedProfit;

    OrdersStruct(uint amout = 0, QDate date = QDate().currentDate(), double totalPrice = 0,
                 CustomerStruct customer = CustomerStruct(), QString fuelType = nullptr, uint establishedProfit = 0)
        : amount(amout)
        , date(date)
        , totalPrice(totalPrice)
        , customer(customer)
        , fuelType(fuelType)
        , establishedProfit(establishedProfit)
    {
    }
};

} // namespace Common

#endif // COMMON_H
