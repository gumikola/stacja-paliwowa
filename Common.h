#ifndef COMMON_H
#define COMMON_H
#include <QString>

namespace Common {

enum class FuelType { ON, OO, PB95, PB98, ERR };

QString getFuelTypeName(const FuelType &type);

}  // namespace Common

#endif  // COMMON_H
