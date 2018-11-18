#ifndef DATABASEAPI_H
#define DATABASEAPI_H
#include <QDate>
#include <QMap>
#include <QtSql/QtSql>

#include <Common.h>

namespace DataBaseApi {

class DataBaseApi {
 private:
  const QString driverName = "QSQLITE";
  const QString pathDatabase = "../stacja-paliwowa/sqlite.db";

  QSqlDatabase driverDatabase;

 public:
  DataBaseApi();
  // TODO: Add table with current price of petrol
  QMap<QString, QMap<QString, uint32_t>> getTanksFillLevel(void);
  QVector<Common::OrdersStruct> getOrdersByDate(QDate date);
};

}  // namespace DataBaseApi

#endif  // DATABASEAPI_H
