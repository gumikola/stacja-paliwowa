#ifndef CALCULATEROUTE_H
#define CALCULATEROUTE_H
#include "Common.h"
#include <QDebug>
#include <QException>
#include <QJsonObject>
#include <QString>
#include <QTime>
#include <QVector>
#include <QtNetwork>
namespace Algorithms {
class CalculateRoute
{
    QByteArray                          jsonResponse;
    const QVector<Common::OrdersStruct> mCustomers;

public:
    CalculateRoute(const QVector<Common::OrdersStruct> Customers);
    QVector<Common::OrdersStruct> CalculateOrder();
};
} // namespace Algorithms
#endif // CALCULATEROUTE_H
