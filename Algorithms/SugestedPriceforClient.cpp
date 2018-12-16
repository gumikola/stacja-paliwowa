#include "SugestedPriceForClient.h"
#include "Algorithms/GetFuelPriceFromWeb.h"
#include "Common.h"
#include "GoogleApi.h"
#include <QtDebug>

namespace Algorithms {

SugestedPriceForClient::SugestedPriceForClient(Common::OrdersStruct& order)
    : mQuantity(order.amount)
    , mEstablishedProfit(order.establishedProfit)
    , mSalaryForDriverPerHour(16)
    , mDeprecationCostPerKilometer(0.1)
    , mAverageFuelConsumptiom(25.0)
    , mCustomer(order.customer)
{
}

uint SugestedPriceForClient::GetPriceForClient(void)
{
    double costs          = GetSalaryCost() + GetDeprececatCost() + GetFuelCost();
    uint   prizeForClient = static_cast<uint>(GetWholesalePrice() + costs + mEstablishedProfit);
    return prizeForClient;
}

double SugestedPriceForClient::GetSalaryCost(void)
{
    return (mSalaryForDriverPerHour * (mOutputOrder.mTravelTime.hour() + mOutputOrder.mTravelTime.minute() / 60));
}

double SugestedPriceForClient::GetDeprececatCost(void)
{
    return (mDeprecationCostPerKilometer * mOutputOrder.mDistance);
}

double SugestedPriceForClient::GetFuelCost(void)
{
    Algorithms::GetFuelPriceFromWeb price;
    return ((mOutputOrder.mDistance * mAverageFuelConsumptiom) / 100) * price.GetONPrice();
}

double SugestedPriceForClient::GetWholesalePrice(void)
{
    Algorithms::GetFuelPriceFromWeb price;
    return (mQuantity * price.GetONPrice());
}

Common::OrderParametersOutput SugestedPriceForClient::GetOrderStruct(void)
{
    return mOutputOrder;
}

void SugestedPriceForClient::CalculateParameters()
{
    Algorithms::GoogleApi customer(mCustomer);
    QTime                 time(0, 0, 0);
    mOutputOrder.mDistance      = customer.GetDistanceFromJson().toDouble();
    mOutputOrder.mTravelTime    = time.addSecs(customer.GetTimeFromJson().toInt());
    mOutputOrder.mTotalPrice    = GetPriceForClient();
    mOutputOrder.mPricePerLiter = static_cast<double>(mOutputOrder.mTotalPrice) / static_cast<double>(mQuantity);
}
} // namespace Algorithms
