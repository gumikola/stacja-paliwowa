#ifndef SUGESTEDPRICEFORCLIENT_H
#define SUGESTEDPRICEFORCLIENT_H
#include <Common.h>
#include <QVector>
#include <cstdlib>
#include "Common.h"

namespace Algorithms {

class SugestedPriceForClient {
  const uint &mQuantity;
  const uint &mEstablishedProfit;
  const double mSalaryForDriverPerHour;
  const double mDeprecationCostPerKilometer;  //#TODO
  const double mAverageFuelConsumptiom;
  const Common::CustomerStruct &mCustomer;
  Common::OrderParametersOutput OutputOrder;


 public:
  SugestedPriceForClient(Common::OrdersStruct &order);
  uint GetPriceForClient(void);
  double GetSalaryCost(void);
  double GetDeprececatCost(void);
  double GetDistanceToCielnt(void);
  double GetFuelCost(void);
  double GetWholesalePrice(void);
  Common::OrderParametersOutput GetOrderStruct(void);
};
}  // namespace Algorithms
#endif  // SUGESTEDPRICEFORCLIENT_H
