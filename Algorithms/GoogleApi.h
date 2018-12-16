#ifndef GOOGLEAPI_H
#define GOOGLEAPI_H
#include <functional>
#include "Common.h"
#include <QDebug>
#include <QException>
#include <QJsonObject>
#include <QString>
#include <QTime>
#include <QtNetwork>
namespace Algorithms {
class GoogleApi
{
    QByteArray                    jsonResponse;
    const Common::CustomerStruct& mCustomer;
    void                          CheckResponse(QJsonDocument response);

public:
    GoogleApi(const Common::CustomerStruct& customer);
    QString GetDistanceFromJson();
    QString GetTimeFromJson();
};
} // namespace Algorithms
#endif // GOOGLEAPI_H
