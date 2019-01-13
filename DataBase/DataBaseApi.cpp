#include "DataBaseApi.h"

namespace DataBaseApi {
DataBaseApi::DataBaseApi()
{
    qDebug() << __PRETTY_FUNCTION__;

    driverDatabase = QSqlDatabase::addDatabase(driverName);

    QString absolutePathDatabase = QDir::currentPath().split("build")[0] + pathDatabase;
    qDebug() << absolutePathDatabase;
    driverDatabase.setDatabaseName(absolutePathDatabase);

    // Checking DB connection
    if (driverDatabase.open() == false)
    {
        qDebug() << "Error: connection with database fail";
    }
}

DataBaseApi::~DataBaseApi()
{
    driverDatabase.close();
}

QMap<Common::FuelTankType, uint32_t> DataBaseApi::getTanksFillLevel(void)
{
    qDebug() << __PRETTY_FUNCTION__;

    QMap<Common::FuelTankType, uint32_t> data;
    QSqlQuery                            q;

    q.prepare("SELECT * FROM Magazyny ");

    if (q.exec())
    {
        while (q.next())
        {
            data.insert(Common::getFuelTankEnum(q.value("Zbiornik").toString()),
                        q.value("Zawartosc").toUInt());
        }
    }
    else
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }

    return data;
}

QVector<Common::OrdersStruct> DataBaseApi::getOrdersByDate(const QDate& date)
{
    qDebug() << __PRETTY_FUNCTION__;

    QVector<Common::OrdersStruct> data;
    QSqlQuery                     q;

    q.prepare(QString("SELECT * FROM Zamowienia left join Klienci_hurtowi on "
                      "Zamowienia.Klienci_hurtowi_ID=Klienci_hurtowi.ID WHERE "
                      "`Data` LIKE '%1';")
                  .arg(date.toString("'%'yyyy-MM-dd'%'")));

    if (q.exec())
    {
        while (q.next())
        {
            data.push_back(Common::OrdersStruct(
                q.value("Ilosc").toUInt(), q.value("Data").toDate(), q.value("Cena").toDouble(),
                Common::CustomerStruct(q.value("Odbiorca").toString(), q.value("Miasto").toString(),
                                       q.value("Ulica").toString(), q.value("Numer").toString()),
                Common::getFuelTypeEnum(q.value("Typ_Paliwa_Nazwa").toString()),
                q.value("Przychod").toUInt()));
        }
    }
    else
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }

    return data;
}

QVector<Common::CustomerStruct> DataBaseApi::getCustomers()
{
    qDebug() << __PRETTY_FUNCTION__;

    QVector<Common::CustomerStruct> clients;

    QSqlQuery q;

    q.prepare("SELECT * FROM Klienci_hurtowi order by Odbiorca;");

    if (q.exec())
    {
        while (q.next())
        {
            Common::CustomerStruct tmp;
            tmp.name           = q.value("Odbiorca").toString();
            tmp.city           = q.value("Miasto").toString();
            tmp.street         = q.value("Ulica").toString();
            tmp.propertyNumber = q.value("Numer").toString();

            clients.push_back(tmp);
        }
    }
    else
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }

    return clients;
}

void DataBaseApi::editCustomer(const Common::CustomerStruct& prev,
                               const Common::CustomerStruct& actual)
{
    qDebug() << __PRETTY_FUNCTION__;

    QSqlQuery q;

    q.prepare("UPDATE Klienci_hurtowi SET Odbiorca=?, Miasto=?, Ulica=?, Numer=? WHERE Odbiorca=? "
              "AND Miasto=? AND Ulica=? AND Numer=?;");
    q.bindValue(0, actual.name);
    q.bindValue(1, actual.city);
    q.bindValue(2, actual.street);
    q.bindValue(3, actual.propertyNumber);

    q.bindValue(4, prev.name);
    q.bindValue(5, prev.city);
    q.bindValue(6, prev.street);
    q.bindValue(7, prev.propertyNumber);

    q.exec();
    if (q.lastError().isValid())
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }
}

int DataBaseApi::addCustomer(const Common::CustomerStruct& customer)
{
    qDebug() << __PRETTY_FUNCTION__;

    QSqlQuery q;

    q.prepare("INSERT OR REPLACE INTO "
              "`Klienci_hurtowi`(`Odbiorca`,`Miasto`,`Ulica`,`Numer`) VALUES "
              "(?,?,?,?);");
    q.bindValue(0, customer.name);
    q.bindValue(1, customer.city);
    q.bindValue(2, customer.street);
    q.bindValue(3, customer.propertyNumber);

    q.exec();
    if (q.lastError().isValid())
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }

    return q.lastInsertId().toInt();
}

void DataBaseApi::addOrder(const Common::OrdersStruct& order)
{
    qDebug() << __PRETTY_FUNCTION__;

    QSqlQuery q;

    q.prepare("INSERT INTO "
              "`Zamowienia`(`Ilosc`,`Data`,`Cena`,`Klienci_hurtowi_ID`,`Typ_paliwa_"
              "Nazwa`,`Przychod`) VALUES (?,?,?,?,?,?);");
    q.bindValue(0, order.amount);
    q.bindValue(1, order.date);
    q.bindValue(2, order.totalPrice);
    q.bindValue(3, addCustomer(order.customer));
    q.bindValue(4, Common::getFuelTypeName(order.fuelType));
    q.bindValue(5, order.establishedProfit);

    q.exec();
    if (q.lastError().isValid())
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }
}

void DataBaseApi::updateTankFillLevel(const Common::FuelTankType& tank, double number)
{
    qDebug() << __PRETTY_FUNCTION__;

    QSqlQuery q;

    q.prepare("UPDATE Magazyny SET `Zawartosc` = ? WHERE `Zbiornik` = ?;");
    q.bindValue(0, number);
    q.bindValue(1, Common::getFuelTankTypeName(tank));

    q.exec();
    if (q.lastError().isValid())
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }
}

void DataBaseApi::addPriceOfPetrol(const Common::PetrolInfoStruct& info)
{
    qDebug() << __PRETTY_FUNCTION__;

    QSqlQuery q;

    q.prepare("INSERT INTO "
              "`Cena_paliwa`(`Data`,`Cena`,`Typ_paliwa`) VALUES (?,?,?);");
    q.bindValue(0, info.date);
    q.bindValue(1, info.price);
    q.bindValue(2, Common::getFuelTypeName(info.fuelType));

    q.exec();
    if (q.lastError().isValid())
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }
}

QVector<Common::PetrolInfoStruct> DataBaseApi::getPriceOfPetrol(const uint nbrOfElements,
                                                                const Common::FuelType fuelType)
{
    qDebug() << __PRETTY_FUNCTION__;

    QVector<Common::PetrolInfoStruct> data;
    QSqlQuery                         q;

    q.prepare(QString("select * from Cena_paliwa where Typ_paliwa=(?) order by Data LIMIT (?);"));
    q.bindValue(0, Common::getFuelTypeName(fuelType));
    q.bindValue(1, nbrOfElements);

    if (q.exec())
    {
        data.push_back(
            Common::PetrolInfoStruct(q.value("Cena").toDouble(), q.value("Data").toDate(),
                                     Common::getFuelTypeEnum(q.value("Typ_paliwa").toString())));
        while (q.next())
        {
            data.push_back(Common::PetrolInfoStruct(
                q.value("Cena").toDouble(), q.value("Data").toDate(),
                Common::getFuelTypeEnum(q.value("Typ_paliwa").toString())));
        }
    }
    else
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }

    return data;
}

void DataBaseApi::removeCustomer(const Common::CustomerStruct& customer)
{
    qDebug() << __PRETTY_FUNCTION__;

    QSqlQuery q;

    q.prepare(QString(
        "DELETE FROM Klienci_hurtowi WHERE Odbiorca=? AND Miasto=? AND Ulica=? AND Numer=?;"));

    q.bindValue(0, customer.name);
    q.bindValue(1, customer.city);
    q.bindValue(2, customer.street);
    q.bindValue(3, customer.propertyNumber);

    q.exec();
    if (q.lastError().isValid())
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }
}

uint DataBaseApi::getCustomerId(const Common::CustomerStruct& customer)
{
    qDebug() << __PRETTY_FUNCTION__;

    QSqlQuery q;

    q.prepare("SELECT ID FROM Klienci_hurtowi WHERE Odbiorca = ? and Miasto = ? and Ulica = ? and "
              "Numer = ?;");
    q.bindValue(0, customer.name);
    q.bindValue(1, customer.city);
    q.bindValue(2, customer.street);
    q.bindValue(3, customer.propertyNumber);

    if (q.exec())
    {
        if (q.next())
            return q.value("ID").toUInt();
        else
            throw QString("Database: Client not found!");
    }
    else
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }
}

QVector<Common::DistancesStruct> DataBaseApi::getAllDistances()
{
    qDebug() << __PRETTY_FUNCTION__;

    QVector<Common::DistancesStruct> data;
    QSqlQuery                        q;

    q.prepare("SELECT a,b,czas from Trasy;");

    if (q.exec())
    {
        while (q.next())
        {
            data.push_back(Common::DistancesStruct(q.value("a").toUInt(), q.value("b").toUInt(),
                                                   q.value("czas").toUInt()));
        }
    }
    else
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }

    return data;
}

void DataBaseApi::addTravelTime(const Common::DistancesStruct& data)
{
    qDebug() << __PRETTY_FUNCTION__;

    QSqlQuery q;

    q.prepare("INSERT OR REPLACE INTO Trasy(czas, a, b) VALUES (?,?,?);");
    q.bindValue(0, data.time);
    q.bindValue(1, data.a);
    q.bindValue(2, data.b);

    q.exec();
    if (q.lastError().isValid())
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }
}
void DataBaseApi::addTravelTimeFromCompany(const Common::CustomerStruct& customer, uint travelTime)
{
    qDebug() << __PRETTY_FUNCTION__;

    QSqlQuery q;

    q.prepare("INSERT OR REPLACE INTO Trasy(czas, a, b) VALUES (?,?,?);");
    q.bindValue(0, travelTime);
    q.bindValue(1, 1);
    q.bindValue(2, DataBaseApi::getCustomerId(customer));

    q.exec();
    if (q.lastError().isValid())
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }
}

QVector<Common::PurchaseStruct>
DataBaseApi::getClientPurchases(const Common::CustomerStruct& customer)
{
    qDebug() << __PRETTY_FUNCTION__;

    QVector<Common::PurchaseStruct> data;
    QSqlQuery                       q;

    q.prepare("SELECT * FROM Zakupy_klientow inner join Klienci_hurtowi "
              "ON Zakupy_klientow.Klienci_hurtowi_ID = Klienci_hurtowi.ID "
              "WHERE  Klienci_hurtowi.Odbiorca = ?  and Klienci_hurtowi.Miasto = ? and "
              "Klienci_hurtowi.Ulica = ? and Klienci_hurtowi.Numer = ?;");

    q.bindValue(0, customer.name);
    q.bindValue(1, customer.city);
    q.bindValue(2, customer.street);
    q.bindValue(3, customer.propertyNumber);

    if (q.exec())
    {
        while (q.next())
        {
            data.push_back(Common::PurchaseStruct(q.value("Data").toDate(),
                                                  q.value("Produkty_na_stacji_Nazwa").toString()));
        }
    }
    else
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }

    return data;
}

void DataBaseApi::addPurchase(const Common::PurchaseStruct& purchase,
                              const Common::CustomerStruct& customer)
{
    qDebug() << __PRETTY_FUNCTION__;

    QSqlQuery q;

    q.prepare("INSERT OR REPLACE INTO Zakupy_klientow(Data, Klienci_hurtowi_ID, "
              "Produkty_na_stacji_Nazwa) VALUES (?, ?, ?);");
    q.bindValue(0, purchase.date);
    q.bindValue(1, getCustomerId(customer));
    q.bindValue(2, purchase.nameOfProduct);

    q.exec();
    if (q.lastError().isValid())
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }
}

QStringList DataBaseApi::getProuducts()
{
    qDebug() << __PRETTY_FUNCTION__;

    QStringList data;
    QSqlQuery   q;

    q.prepare("SELECT * FROM Produkty_na_stacji;");

    if (q.exec())
    {
        while (q.next())
        {
            data.push_back(q.value(0).toString());
        }
    }
    else
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }

    return data;
}

void DataBaseApi::addProduct(const QString& product)
{
    qDebug() << __PRETTY_FUNCTION__;

    QSqlQuery q;

    q.prepare("INSERT or REPLACE INTO Produkty_na_stacji (Nazwa) values (?);");
    q.bindValue(0, product);

    q.exec();
    if (q.lastError().isValid())
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }
}

void DataBaseApi::removeProduct(const QString& product)
{
    qDebug() << __PRETTY_FUNCTION__;

    QSqlQuery q;

    q.prepare("DELETE FROM Produkty_na_stacji WHERE Nazwa=?;");
    q.bindValue(0, product);

    q.exec();
    if (q.lastError().isValid())
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }
}

void DataBaseApi::editProduct(const QString& oldName, const QString& newName)
{
    qDebug() << __PRETTY_FUNCTION__;

    QSqlQuery q;

    q.prepare("UPDATE Produkty_na_stacji SET Nazwa=? where Nazwa=?;");
    q.bindValue(0, newName);
    q.bindValue(1, oldName);

    q.exec();
    if (q.lastError().isValid())
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }
}

void DataBaseApi::removeOrder(const Common::OrdersStruct& order)
{
    qDebug() << __PRETTY_FUNCTION__;
}

} // namespace DataBaseApi
