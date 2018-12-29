#ifndef CALCULATEORDER_H
#define CALCULATEORDER_H
#include "Common.h"
#include <QDebug>
#include <QException>
#include <QJsonObject>
#include <QString>
#include <QTime>
#include <QVector>
#include <QtNetwork>
#include <stdlib.h>
namespace Algorithms {
class CalculateOrder
{
    const QVector<Common::OrdersStruct> mCustomers;

public:
    struct CustomerDistanceID
    {
        Common::CustomerStruct Customer;
        int                    ID;
        bool                   isHome;
        QVector<int>           distancesTable;
    };
    CalculateOrder(const QVector<Common::OrdersStruct> Customers);
    QVector<int> GetDistanceVector(QVector<Common::CustomerStruct> customers, int customerID);
    QVector<QVector<Common::OrdersStruct>> GetOrder();
    QVector<CalculateOrder::CustomerDistanceID>
                    drawPosition(QVector<CalculateOrder::CustomerDistanceID> Customers);
    QPair<int, int> CalculateTime(QVector<CalculateOrder::CustomerDistanceID> Order,
                                  bool                                        twoTrucks);
    QPair<QVector<CalculateOrder::CustomerDistanceID>, int>
    CalculateOrderFor2Or1Trucks(bool twoTrucks);
};
} // namespace Algorithms
#endif // CALCULATEORDER_H
