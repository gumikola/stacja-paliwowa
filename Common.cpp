#include "Common.h"
#include <QString>

namespace Common {

QString getFuelTypeName(const FuelType &type) {
  switch (type) {
    case FuelType::ON:
      return QString("ON");

    case FuelType::OO:
      return QString("OO");

    case FuelType::PB95:
      return QString("PB95");

    case FuelType::PB98:
      return QString("PB98");

    default:
      throw QString("Wrong fuel type");
  }
}

QString getFuelTankTypeName(const FuelTankType &type) {
  switch (type) {
    case FuelTankType::ON:
      return QString("ON");

    case FuelTankType::ON1:
      return QString("ON 1");

    case FuelTankType::ON2:
      return QString("ON 2");

    case FuelTankType::ON3:
      return QString("ON 3");

    case FuelTankType::OO:
      return QString("OO");

    case FuelTankType::PB95:
      return QString("PB95");

    case FuelTankType::PB98:
      return QString("PB98");

    default:
      throw QString("Wrong fuel type");
  }
}

}  // namespace Common
