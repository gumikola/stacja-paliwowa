#ifndef GETFUELPRICEFROMWEB_H
#define GETFUELPRICEFROMWEB_H

#include <QString>
#include <QtNetwork>
#include <functional>
#include <Common.h>

namespace Algorithms {

class GetFuelPriceFromWeb {
  QString mPageSourceCode;

 public:
  GetFuelPriceFromWeb();
  double MatchRegex(QString Regex);
  double Get95Price();
  double Get98Price();
  double GetONPrice();
  double GetOOPrice();
};
}  // namespace Algorithms
#endif  // GETFUELPRICEFROMWEB_H
