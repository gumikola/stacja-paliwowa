#ifndef DATABASEAPI_H
#define DATABASEAPI_H
#include <QDate>
#include <QMap>
#include <QtSql/QtSql>

namespace DataBaseApi {
class DataBaseApi {
 private:
  QString driverName;
  QString pathDatabase;

  QSqlDatabase driverDatabase;

 public:
  DataBaseApi();

  // TODO: Get all order from choosed date
  QMap<QString, QMap<QString, uint32_t>> getTanksFillLevel(void);
};

}  // namespace databaseApi

#endif  // DATABASEAPI_H
