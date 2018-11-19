#include "DataBaseApi.h"

namespace DataBaseApi {
DataBaseApi::DataBaseApi() {
  driverDatabase = QSqlDatabase::addDatabase(driverName);
  driverDatabase.setDatabaseName(pathDatabase);

  // Checking DB connection
  if (driverDatabase.open() == false) {
    qDebug() << "Error: connection with database fail";
  }
}

QMap<QString, QMap<QString, uint32_t> > DataBaseApi::getTanksFillLevel(void) {
  QMap<QString, QMap<QString, uint32_t> > data;
  QSqlQuery q;

  q.prepare("SELECT * FROM Magazyny ");

  if (q.exec()) {
    QMap<QString, uint> mapTmp;
    QString stringTmp;
    while (q.next()) {
      stringTmp = q.value("Typ_paliwa_Nazwa").toString() + QString(" ") +
                  q.value("ID").toString();
      mapTmp.insert(stringTmp, q.value("Zawartosc").toUInt());
      data.insert(q.value("Miejsce").toString(), mapTmp);
    }
  }

  return data;
}

QVector<Common::OrdersStruct> DataBaseApi::getOrdersByDate(QDate date) {
  QVector<Common::OrdersStruct> data;
  QSqlQuery q;

  q.prepare(QString("SELECT * FROM Zamowienia left join Klienci_hurtowi on "
                    "Zamowienia.Klienci_hurtowi_ID=Klienci_hurtowi.ID WHERE "
                    "`Data` LIKE '%1'")
                .arg(date.toString("'%'yyyy-MM-dd'%'")));

  if (q.exec()) {
    while (q.next()) {
      data.push_back(Common::OrdersStruct(
          q.value("Ilosc").toUInt(), q.value("Data").toDate(),
          q.value("Cena").toDouble(),
          Common::CustomerStruct(
              q.value("Odbiorca").toString(), q.value("Miasto").toString(),
              q.value("Ulica").toString(), q.value("Numer").toString()),
          q.value("Typ_Paliwa_Nazwa").toString()));
    }
  }

  return data;
}

}  // namespace DataBaseApi
