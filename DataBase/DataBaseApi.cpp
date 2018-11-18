#include "DataBaseApi.h"

namespace DataBaseApi {
DataBaseApi::DataBaseApi()
    : driverName("QSQLITE"),
      pathDatabase("/home/mguz/workspace/stacja-paliwowa/sqlite.db") {
  driverDatabase = QSqlDatabase::addDatabase(driverName);
  driverDatabase.setDatabaseName(pathDatabase);

  // Checking DB connection
  if (driverDatabase.open() == false) {
    qDebug() << "Error: connection with database fail";
  }
}

QMap<QString, QMap<QString, uint32_t> > DataBaseApi::getTanksFillLevel(void) {
  QMap<QString, QMap<QString, uint32_t> > data;
  QSqlQuery query;

  query.prepare("SELECT * FROM Magazyny ");

  if (query.exec()) {
    QMap<QString, uint> tmp;
    while (query.next()) {
      tmp.insert(query.value("Typ_paliwa_Nazwa").toString() + " " +
                     query.value("ID").toString(),
                 query.value("Zawartosc").toUInt());
      data.insert(query.value("Miejsce").toString(), tmp);
    }
  }

  return data;
}
}  // namespace databaseApi
