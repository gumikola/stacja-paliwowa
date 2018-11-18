#ifndef COMMON_H
#define COMMON_H
#include <QDate>
#include <QString>
#include <QTime>

namespace Common {

enum class FuelType { ON, OO, PB95, PB98, ERR };

QString getFuelTypeName(const FuelType &type);

}  // namespace Common

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

#endif  // COMMON_H
