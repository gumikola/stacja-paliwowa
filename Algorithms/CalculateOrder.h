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
    const QVector<Common::OrdersStruct> mCustomers;
    QVector<uint>                       mIds;
    QVector<Common::Distances>          mDistances;
    void                                GetCustomersID(void);
    void                                checkIfDistanceIsInDatabase(void);

public:
    CalculateOrder(const QVector<Common::OrdersStruct> Customers);
    void CheckDistancesInDatabase(void);
};
} // namespace Algorithms
#endif // CALCULATEORDER_H
