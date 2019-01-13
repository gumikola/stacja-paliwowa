#ifndef CALCULATEORDER_H
#define CALCULATEORDER_H
#include <algorithm>
#include "Common.h"
#include "DataBase/DataBaseApi.h"
#include <QDebug>
#include <QException>
#include <QJsonObject>
#include <QList>
#include <QPair>
#include <QString>
#include <QTime>
#include <QVector>
#include <QtNetwork>
#include <stdlib.h>
namespace Algorithms {
class CalculateOrder
{

    DataBaseApi::DataBaseApi&            mDatabaseApi;
    const QVector<Common::OrdersStruct>& mCustomers;
    QVector<uint>                        mIds;
    QVector<Common::DistancesStruct>     getDistances(void);
    QVector<Common::DistancesStruct>
                                           calculateAdditionalDistances(const QVector<Common::DistancesStruct>& DistancesToCalculate);
    QVector<QVector<Common::OrdersStruct>> sortOrders(QVector<Common::DistancesStruct> distances);
    QVector<QVector<Common::OrdersStruct>> getOrders();
    Common::CustomerStruct                 getCustomerById(uint id);
    QVector<int>  getDistancesFromGoogle(const QString origin, const QString destination,
                                         const uint size);
    uint          getTimeBeetwenTwoPlaces(uint id1, uint id2,
                                          const QVector<Common::DistancesStruct> distancesVector);
    QVector<uint> drawOrder(QVector<uint> Order);
    uint          getFullTime(QVector<uint>                          customers,
                              const QVector<Common::DistancesStruct> distancesVector);
    QVector<QVector<Common::OrdersStruct>> transformIdsToOrders(QVector<QVector<uint>> bestOrder);
    QPair<uint, QVector<uint>>             optimalizeOrder(const uint                       numberOfDraw,
                                                           QVector<Common::DistancesStruct> distances,
                                                           QPair<uint, QVector<uint>>       startOrder);
    QPair<uint, QVector<uint>>             checkIfDrawIsBetter(QPair<uint, QVector<uint>> best,
                                                               QPair<uint, QVector<uint>> tmp);
    QPair<uint, QPair<QVector<uint>, QVector<uint>>>
    checkIfDrawIsBetterFor2(QPair<uint, QVector<uint>>                       bestOrderAndTime1,
                            QPair<uint, QVector<uint>>                       bestOrderAndTime2,
                            QPair<uint, QPair<QVector<uint>, QVector<uint>>> best);

public:
    CalculateOrder(DataBaseApi::DataBaseApi&            databaseApiconst,
                   const QVector<Common::OrdersStruct>& customers);
};
} // namespace Algorithms
#endif // CALCULATEORDER_H
