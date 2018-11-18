#ifndef GETFUELPRICEFROMWEB_H
#define GETFUELPRICEFROMWEB_H

#include <QString>
#include <QtNetwork>
#include <functional>

namespace Algorithms {

class GetFuelPriceFromWeb {
  const QString &mURL;
  QString mPageSourceCode;

 public:
  GetFuelPriceFromWeb(const QString &URL);
  double MatchRegex(QString Regex);
  double Get95Price();
  double Get98Price();
  double GetONPrice();
  double GetOOPrice();
};
}  // namespace Algorithms
#endif  // GETFUELPRICEFROMWEB_H
