#ifndef DATABASEAPI_H
#define DATABASEAPI_H
#include <QDate>
#include <QMap>
#include <QtSql/QtSql>

#include <Common.h>

namespace DataBaseApi {

class DataBaseApi
{
private:
    const QString driverName = "QSQLITE";

#ifdef __linux__
    // miki
    const QString pathDatabase = "../stacja-paliwowa/sqlite.db";
#elif _WIN32
    // luki
    const QString pathDatabase = "E:\\Studia\\9_sem\\Projekty\\System_zarzadzania_stacja_paliw\\stacja-"
                                 "paliwowa\\sqlite.db";
#else
#error Not supported
#endif

    QSqlDatabase driverDatabase;

public:
    DataBaseApi();
    ~DataBaseApi();
    QMap<Common::FuelTankType, uint32_t> getTanksFillLevel(void);
    QVector<Common::OrdersStruct>        getOrdersByDate(QDate date);
    int                                  addCustomer(const Common::CustomerStruct& customer);
    void                                 addOrder(const Common::OrdersStruct& order);
    void                                 updateTankFillLevel(Common::FuelTankType tank, double number);
    void                                 addPriceOfPetrol(double price, QDate date, Common::FuelType type);
};

} // namespace DataBaseApi

#endif // DATABASEAPI_H
