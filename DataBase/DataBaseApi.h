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

  // miki
  const QString pathDatabase = "../stacja-paliwowa/sqlite.db";

  // luki
  //  const QString pathDatabase =
  //      "E:\\Studia\\9_sem\\Projekty\\System_zarzadzania_stacja_paliw\\stacja-"
  //      "paliwowa\\sqlite.db";

  QSqlDatabase driverDatabase;

 public:
  DataBaseApi();
  ~DataBaseApi();
  // TODO: Add table with current price of petrol
  QMap<Common::FuelTankType, uint32_t> getTanksFillLevel(void);
  QVector<Common::OrdersStruct> getOrdersByDate(QDate date);
  void addCustomer(Common::CustomerStruct &order);
  void addOrder(Common::CustomerStruct &customer);
};

}  // namespace DataBaseApi

#endif  // DATABASEAPI_H
