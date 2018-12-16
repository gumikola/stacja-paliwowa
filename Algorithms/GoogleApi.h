#ifndef GOOGLEAPI_H
#define GOOGLEAPI_H
#include <functional>
#include "Common.h"
#include <QDebug>
#include <QJsonObject>
#include <QString>
#include <QTime>
#include <QtNetwork>
namespace Algorithms {
class GoogleApi
{
    QByteArray                    jsonResponse;
    const Common::CustomerStruct& mCustomer;

public:
    GoogleApi(const Common::CustomerStruct& customer);
    QString GetDistanceFromJson();
    QString GetTimeFromJson();
};
} // namespace Algorithms
#endif // GOOGLEAPI_H
