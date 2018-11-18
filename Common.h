#ifndef COMMON_H
#define COMMON_H
#include <QDate>
#include <QString>
#include <QTime>

namespace Common {

enum class FuelType { ON, OO, PB95, PB98, ERR };

QString getFuelTypeName(const FuelType &type);

struct OrderParametersOutput {
  QTime mTravelTime;
  uint mTotalPrice;
  double mMargin;
  double mDistance;
  uint mIncome;

  OrderParametersOutput() {}
  OrderParametersOutput(QTime travelTime, uint totalPrice, double margin,
                        double distance, uint income)
      : mTravelTime(travelTime),
        mTotalPrice(totalPrice),
        mMargin(margin),
        mDistance(distance),
        mIncome(income) {}
};

struct CustomerStruct {
  QString name;
  QString city;
  QString street;
  QString propertyNumber;

  CustomerStruct(QString name = nullptr, QString city = nullptr,
                 QString street = nullptr, QString propertyNumber = nullptr)
      : name(name),
        city(city),
        street(street),
        propertyNumber(propertyNumber) {}
};

struct OrdersStruct {
  uint amout;
  QDate date;
  double totalPrice;
  CustomerStruct customer;
  QString fuelType;

  OrdersStruct(uint amout = 0, QDate date = QDate().currentDate(),
               double totalPrice = 0,
               CustomerStruct customer = CustomerStruct(),
               QString fuelType = nullptr)
      : amout(amout),
        date(date),
        totalPrice(totalPrice),
        customer(customer),
        fuelType(fuelType) {}
};

}  // namespace Common

#endif  // COMMON_H
