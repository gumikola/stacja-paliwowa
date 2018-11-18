#ifndef SUGESTEDPRICEFORCLIENT_H
#define SUGESTEDPRICEFORCLIENT_H
#include <QVector>
#include <cstdlib>
namespace Algorithms {

class SugestedPriceForClient {
  const double &mDistance;
  const uint &mQuantity;
  const uint &mEstablishedProfit;
  const double &mWholesalePricePerLiter;
  const double &mTimeNeeded;
  const double &mSalaryForDriverPerHour;
  const double &mDeprecationCostPerKilometer;
  const double &mAverageFuelConsumptiom;

 public:
  SugestedPriceForClient(const double &distance, const uint &quantity,
                         const uint &establishedProfit,
                         const double &wholesalePricePerLiter,
                         const double &timeNeeded,
                         const double &salaryForDriverPerHour,
                         const double &deprecationCostPerKilometer,
                         const double &averageFuelConsumptiom);
  uint GetPriceForClient(void);
  double GetSalaryCost(void);
  double GetDeprececatCost(void);
  double GetFuelCost(void);
  double GetWholesalePrice(void);
};
}  // namespace Algorithms
#endif  // SUGESTEDPRICEFORCLIENT_H
