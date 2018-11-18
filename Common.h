#ifndef COMMON_H
#define COMMON_H
#include <QString>
#include <QTime>

namespace Common {

enum class FuelType { ON, OO, PB95, PB98, ERR };

QString getFuelTypeName(const FuelType &type);

}  // namespace Common

struct OrderParameters {
  QTime mTravelTime;
  uint mTotalPrice;
  double mMargin;
  double mDistance;
};

#endif  // COMMON_H
