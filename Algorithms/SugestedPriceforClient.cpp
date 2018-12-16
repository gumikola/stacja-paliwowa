#include "SugestedPriceForClient.h"
#include "Algorithms/GetFuelPriceFromWeb.h"
#include <QtDebug>
#include "Common.h"
#include "GoogleApi.h"

namespace Algorithms {

SugestedPriceForClient::SugestedPriceForClient(Common::OrdersStruct &order)
    : mQuantity(order.amount),
      mEstablishedProfit(order.establishedProfit),
      mSalaryForDriverPerHour(16),
      mDeprecationCostPerKilometer(0.1),
      mAverageFuelConsumptiom(25.0),
      mCustomer(order.customer) {}

uint SugestedPriceForClient::GetPriceForClient(void) {
  double costs = GetSalaryCost() + GetDeprececatCost() + GetFuelCost();
  uint prizeForClient =
      static_cast<uint>(GetWholesalePrice() + costs + mEstablishedProfit);
  return prizeForClient;
}

double SugestedPriceForClient::GetSalaryCost(void) {
  return (mSalaryForDriverPerHour *
          (OutputOrder.mTravelTime.hour() + OutputOrder.mTravelTime.minute()/60));
}

double SugestedPriceForClient::GetDeprececatCost(void) {
  return (mDeprecationCostPerKilometer * OutputOrder.mDistance);
}

double SugestedPriceForClient::GetFuelCost(void) {
  Algorithms::GetFuelPriceFromWeb price;
  return ((OutputOrder.mDistance * mAverageFuelConsumptiom) / 100) * price.GetONPrice();
}

double SugestedPriceForClient::GetWholesalePrice(void) {
    Algorithms::GetFuelPriceFromWeb price;
  return (mQuantity * price.GetONPrice());
}

Common::OrderParametersOutput SugestedPriceForClient::GetOrderStruct(void) {
    Algorithms::GoogleApi customer(mCustomer);
    QTime time(0, 0, 0);
    OutputOrder.mDistance = customer.GetDistanceFromJson().toDouble();
    OutputOrder.mTravelTime = time.addSecs(customer.GetTimeFromJson().toInt());
    OutputOrder.mTotalPrice = GetPriceForClient();
    OutputOrder.mMargin = OutputOrder.mTotalPrice - mEstablishedProfit;
    return OutputOrder;

}
}  // namespace Algorithms
