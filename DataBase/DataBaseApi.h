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

    const QString pathDatabase = "stacja-paliwowa/sqlite.db";

    QSqlDatabase driverDatabase;

public:
    DataBaseApi();
    ~DataBaseApi();
    // TODO: Add table with current price of petrol
    QMap<Common::FuelTankType, uint32_t> getTanksFillLevel(void);
    QVector<Common::OrdersStruct>        getOrdersByDate(QDate date);
    QVector<Common::CustomerStruct>      getClients();
    void editCustomer(const Common::CustomerStruct& prev, const Common::CustomerStruct& actual);
    int  addCustomer(const Common::CustomerStruct& customer);
    void addOrder(const Common::OrdersStruct& order);
    void updateTankFillLevel(Common::FuelTankType tank, double number);
    void addPriceOfPetrol(Common::PetrolInfoStruct info);
    QVector<Common::PetrolInfoStruct> getPriceOfPetrol(uint             nbrOfElements,
                                                       Common::FuelType fuelType);
    void                              removeClient(Common::CustomerStruct& customer);
    uint getCustomerId(const Common::CustomerStruct& customer); // for igi
    QVector<Common::DistancesStruct> getAllDistances();         // for igi
    QVector<Common::PurchaseStruct>  getClientPurchases(const Common::CustomerStruct& customer);
    void                             addPurchase(const Common::PurchaseStruct& purchase,
                                                 const Common::CustomerStruct& customer);
    QStringList                      getProuducts();
    void                             addProduct(QString product);
    void                             removeProduct(QString product);
    void                             editProduct(QString oldName, QString newName);
};

} // namespace DataBaseApi

#endif // DATABASEAPI_H
