#ifndef CALCULATEORDER_H
#define CALCULATEORDER_H
#include "Common.h"
#include "DataBase/DataBaseApi.h"
#include <QDebug>
#include <QException>
#include <QJsonObject>
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
    bool checkIfDistanceIsInDatabase(const Common::DistancesStruct& DistanceFromDatabase,
                                     const uint& a, const uint& b);
    QVector<Common::DistancesStruct> GetDistances(void);
    QVector<Common::DistancesStruct>
                                  CalculateAdditionalDistances(const QVector<Common::DistancesStruct>& DistancesToCalculate);
    void                          SortOrders(QVector<Common::DistancesStruct> distances);
    QVector<Common::OrdersStruct> GetOrders();
    Common::CustomerStruct        getCustomerById(uint id);
    QVector<int> getDistancesFromGoogle(const QString origin, const QString destination,
                                        const uint size);

public:
    CalculateOrder(DataBaseApi::DataBaseApi&            databaseApiconst,
                   const QVector<Common::OrdersStruct>& customers);
};
} // namespace Algorithms
#endif // CALCULATEORDER_H
