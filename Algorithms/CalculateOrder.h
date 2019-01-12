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
    bool checkIfDistanceIsInDatabase(const Common::DistancesStruct& DistanceFromDatabase,
                                     const uint& a, const uint& b);
    QVector<Common::DistancesStruct> GetDistances(void);
    QVector<Common::DistancesStruct>
                                           CalculateAdditionalDistances(const QVector<Common::DistancesStruct>& DistancesToCalculate);
    QVector<QVector<Common::OrdersStruct>> SortOrders(QVector<Common::DistancesStruct> distances);
    QVector<QVector<Common::OrdersStruct>> GetOrders();
    Common::CustomerStruct                 getCustomerById(uint id);
    QVector<int>  getDistancesFromGoogle(const QString origin, const QString destination,
                                         const uint size);
    uint          getTimeBeetwenTwoPlaces(uint id1, uint id2,
                                          const QVector<Common::DistancesStruct> distancesVector);
    QVector<uint> drawOrder(QVector<uint> Order);
    uint          getFullTime(QVector<uint>                          customers,
                              const QVector<Common::DistancesStruct> distancesVector);
    QVector<QVector<Common::OrdersStruct>> transformIdsToOrders(QVector<QVector<uint>> bestOrder);

public:
    CalculateOrder(DataBaseApi::DataBaseApi&            databaseApiconst,
                   const QVector<Common::OrdersStruct>& customers);
};
} // namespace Algorithms
#endif // CALCULATEORDER_H
