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
    void updateTankFillLevel(const Common::FuelTankType& tank, double number);

    QVector<Common::OrdersStruct> getOrdersByDate(const QDate& date);
    void                          addOrder(const Common::OrdersStruct& order);
    void                          removeOrder(const Common::OrdersStruct& order);

    QVector<Common::CustomerStruct> getCustomers();
    void editCustomer(const Common::CustomerStruct& prev, const Common::CustomerStruct& actual);
    int  addCustomer(const Common::CustomerStruct& customer);
    void removeCustomer(const Common::CustomerStruct& customer);
    uint getCustomerId(const Common::CustomerStruct& customer);
    Common::CustomerStruct getCustomerById(uint id);

    void                              addPriceOfPetrol(const Common::PetrolInfoStruct& info);
    QVector<Common::PetrolInfoStruct> getPriceOfPetrol(const uint             nbrOfElements,
                                                       const Common::FuelType fuelType);

    QVector<Common::DistancesStruct> getAllDistances();
    void                             addTravelTime(const Common::DistancesStruct& data);
    void addTravelTimeFromCompany(const Common::CustomerStruct& customer, uint travelTime);

    QVector<Common::PurchaseStruct> getClientPurchases(const Common::CustomerStruct& customer);
    void                            addPurchase(const Common::PurchaseStruct& purchase,
                                                const Common::CustomerStruct& customer);

    QStringList getProuducts();
    void        addProduct(const QString& product);
    void        removeProduct(const QString& product);
    void        editProduct(const QString& oldName, const QString& newName);
};

} // namespace DataBaseApi

#endif // DATABASEAPI_H
