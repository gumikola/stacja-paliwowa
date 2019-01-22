#ifndef CALCULATEORDER_H
#define CALCULATEORDER_H
#include <algorithm>
#include "Common.h"
#include "DataBase/DataBaseApi.h"
#include <QtNetwork>

namespace Algorithms {
class CalculateOrder
{

    DataBaseApi::DataBaseApi&   mDatabaseApi;
    QList<Common::OrdersStruct> mCustomers;
    QList<uint>                 mIds;
    QList<uint>                 drawOrder(QList<uint>& order);
    QList<int> getDistancesFromGoogle(const QString origin, const QString destination,
                                      const uint size);

    uint getTimeBeetwenTwoPlaces(uint id1, uint id2,
                                 const QList<Common::DistancesStruct> distancesVector);
    uint getFullTime(QList<uint> customers, const QList<Common::DistancesStruct> distancesVector);

    QList<Common::DistancesStruct> getDistances(void);
    QList<Common::DistancesStruct>
    calculateAdditionalDistances(const QList<Common::DistancesStruct>& distancesToCalculate);

    QList<QList<Common::OrdersStruct>> sortOrders(QList<Common::DistancesStruct> distances);

    Common::CustomerStruct getCustomerById(uint id);
    Common::OrdersStruct   getOrderByCustomerId(uint id);

    QList<QList<Common::OrdersStruct>> transformIdsToOrders(QList<QList<uint>> order);

    QPair<uint, QList<uint>> optimalizeOrder(QList<Common::DistancesStruct> distances,
                                             QList<uint>                    order);
    QPair<uint, QList<uint>> checkIfDrawIsBetter(QPair<uint, QList<uint>> best,
                                                 QPair<uint, QList<uint>> tmp);
    QPair<uint, QPair<QList<uint>, QList<uint>>>
    checkIfDrawIsBetterFor2(QPair<uint, QList<uint>>                     bestOrderAndTime1,
                            QPair<uint, QList<uint>>                     bestOrderAndTime2,
                            QPair<uint, QPair<QList<uint>, QList<uint>>> best);

    QList<QList<uint>> divideOrderFor2Trucks(QPair<uint32_t, QList<uint>> bestOrderAndTime);
    bool               checkIfBenzolIsInOrder(QList<uint>& order);

public:
    CalculateOrder(DataBaseApi::DataBaseApi&          databaseApiConst,
                   const QList<Common::OrdersStruct>& customers);
    QList<QList<Common::OrdersStruct>> getOrders();
};
} // namespace Algorithms
#endif // CALCULATEORDER_H
