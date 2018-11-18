#include "SugestedPriceForClient.h"
#include <QtDebug>

namespace Algorithms {

SugestedPriceForClient::SugestedPriceForClient(
    const double &distance, const uint &quantity, const uint &establishedProfit,
    const double &wholesalePricePerLiter, const double &timeNeeded,
    const double &salaryForDriverPerHour,
    const double &deprecationCostPerKilometer,
    const double &averageFuelConsumptiom)
    : mDistance(distance),
      mQuantity(quantity),
      mEstablishedProfit(establishedProfit),
      mWholesalePricePerLiter(wholesalePricePerLiter),
      mTimeNeeded(timeNeeded),
      mSalaryForDriverPerHour(salaryForDriverPerHour),
      mDeprecationCostPerKilometer(deprecationCostPerKilometer),
      mAverageFuelConsumptiom(averageFuelConsumptiom) {}

uint SugestedPriceForClient::GetPriceForClient(void) {
  double costs = GetSalaryCost() + GetDeprececatCost() + GetFuelCost();
  uint prizeForClient =
      static_cast<uint>(GetWholesalePrice() + costs + mEstablishedProfit);
  return prizeForClient;
}

double SugestedPriceForClient::GetSalaryCost(void) {
  return (mSalaryForDriverPerHour * mTimeNeeded);
}

double SugestedPriceForClient::GetDeprececatCost(void) {
  return (mDeprecationCostPerKilometer * mDistance);
}

double SugestedPriceForClient::GetFuelCost(void) {
  return ((mDistance * mAverageFuelConsumptiom) / 100) *
         mWholesalePricePerLiter;
}

double SugestedPriceForClient::GetWholesalePrice(void) {
  return (mQuantity * mWholesalePricePerLiter);
}
}  // namespace Algorithms
