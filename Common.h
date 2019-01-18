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
static QString chartsPricesDataPath = "stacja-paliwowa/FuelPricesChartData/";

QString      getFuelTypeName(const FuelType& type);
FuelType     getFuelTypeEnum(QString type);
QString      getFuelTankTypeName(const FuelTankType& type);
FuelTankType getFuelTankEnum(QString type);

struct OrderParametersOutput
{
    QTime  mTravelTime;
    uint   mTotalPrice;
    double mPricePerLiter;
    double mDistance;
    uint   mIncome;

    OrderParametersOutput() {}
    OrderParametersOutput(QTime travelTime, uint totalPrice, double margin, double distance,
                          uint income)
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
    uint    id;

    CustomerStruct(QString name = nullptr, QString city = nullptr, QString street = nullptr,
                   QString propertyNumber = nullptr, uint id = 0)
        : name(name)
        , city(city)
        , street(street)
        , propertyNumber(propertyNumber)
        , id(id)
    {
    }
};
struct OrdersStruct
{
    uint           amount;
    QDate          date;
    double         totalPrice;
    CustomerStruct customer;
    FuelType       fuelType;
    uint           establishedProfit;

    OrdersStruct(uint amout = 0, QDate date = QDate().currentDate(), double totalPrice = 0,
                 CustomerStruct customer = CustomerStruct(), FuelType fuelType = FuelType::ERR,
                 uint establishedProfit = 0)
        : amount(amout)
        , date(date)
        , totalPrice(totalPrice)
        , customer(customer)
        , fuelType(fuelType)
        , establishedProfit(establishedProfit)
    {
    }
};

struct DistancesStruct
{
    uint a;
    uint b;
    uint time;

    DistancesStruct(uint a = 0, uint b = 0, uint time = 0)
        : a(a)
        , b(b)
        , time(time)
    {
    }
};

struct PetrolInfoStruct
{
    double   price;
    QDate    date;
    FuelType fuelType;

    PetrolInfoStruct(double price = 0, QDate date = QDate().currentDate(),
                     FuelType fuelType = FuelType::ERR)
        : price(price)
        , date(date)
        , fuelType(fuelType)
    {
    }
};

struct PurchaseStruct
{
    QDate   date;
    QString nameOfProduct;

    PurchaseStruct(QDate date = QDate().currentDate(), QString nameOfProduct = QString())
        : date(date)
        , nameOfProduct(nameOfProduct)
    {
    }
};

} // namespace Common

#endif // COMMON_H
