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

}  // namespace Common
