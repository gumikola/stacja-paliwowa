#ifndef GOOGLEAPI_H
#define GOOGLEAPI_H
#include <QJsonObject>
#include <QString>
#include <QTime>
#include <QtNetwork>
#include <functional>
#include "Common.h"
#include <QDebug>
namespace Algorithms {
class GoogleApi {
  QByteArray jsonResponse;
  const Common::CustomerStruct &mCustomer;

 public:
  GoogleApi(const Common::CustomerStruct &customer);
  QString GetDistanceFromJson();
  QString GetTimeFromJson();
};
}  // namespace Algorithms
#endif  // GOOGLEAPI_H
