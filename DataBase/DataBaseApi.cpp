#include "DataBaseApi.h"

namespace DataBaseApi {
DataBaseApi::DataBaseApi()
{
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

QVector<Common::OrdersStruct> DataBaseApi::getOrdersByDate(QDate date)
{
    QVector<Common::OrdersStruct> data;
    QSqlQuery                     q;

    q.prepare(QString("SELECT * FROM Zamowienia left join Klienci_hurtowi on "
                      "Zamowienia.Klienci_hurtowi_ID=Klienci_hurtowi.ID WHERE "
                      "`Data` LIKE '%1'")
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
                q.value("Przychod").toDouble()));
        }
    }
    else
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }

    return data;
}

QVector<Common::CustomerStruct> DataBaseApi::getClients()
{
    QVector<Common::CustomerStruct> clients;

    QSqlQuery q;

    q.prepare("SELECT * FROM Klienci_hurtowi order by Odbiorca");

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
    QSqlQuery q;

    q.prepare("INSERT OR REPLACE INTO "
              "`Klienci_hurtowi`(`Odbiorca`,`Miasto`,`Ulica`,`Numer`) VALUES "
              "(?,?,?,?)");
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
    QSqlQuery q;

    q.prepare("INSERT INTO "
              "`Zamowienia`(`Ilosc`,`Data`,`Cena`,`Klienci_hurtowi_ID`,`Typ_paliwa_"
              "Nazwa`,`Marza`) VALUES (?,?,?,?,?,?)");
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

void DataBaseApi::updateTankFillLevel(Common::FuelTankType tank, double number)
{
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

void DataBaseApi::addPriceOfPetrol(Common::PetrolInfoStruct info)
{
    QSqlQuery q;

    q.prepare("INSERT INTO "
              "`Cena_paliwa`(`Data`,`Cena`,`Typ_paliwa`) VALUES (?,?,?)");
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

QVector<Common::PetrolInfoStruct> DataBaseApi::getPriceOfPetrol(uint             nbrOfElements,
                                                                Common::FuelType fuelType)
{
    QVector<Common::PetrolInfoStruct> data;
    QSqlQuery                         q;

    q.prepare(QString("select * from Cena_paliwa where Typ_paliwa=(?) order by Data LIMIT (?)"));
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

void DataBaseApi::removeClient(Common::CustomerStruct& customer)
{
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

uint DataBaseApi::GetCustomerId(Common::CustomerStruct customer)
{
    QSqlQuery q;

    q.prepare("SELECT ID FROM Klienci_hurtowi WHERE Odbiorca = ? and Miasto = ? and Ulica = ? and "
              "Numer = ?");
    q.bindValue(0, customer.name);
    q.bindValue(1, customer.city);
    q.bindValue(2, customer.street);
    q.bindValue(4, customer.propertyNumber);

    if (q.exec())
    {
        if (q.next())
            return q.value(0).toUInt();
        else
            throw QString("Database: Client not found!");
    }
    else
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }
    return 0;
}

QVector<Common::DistancesStruct> DataBaseApi::GetAllDistances()
{
    QVector<Common::DistancesStruct> data;
    QSqlQuery                        q;

    q.prepare("SELECT a,b,czas from Trasy");

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

QVector<Common::PurchaseStruct> DataBaseApi::getClientPurchases(Common::CustomerStruct customer)
{
    QVector<Common::PurchaseStruct> data;
    QSqlQuery                       q;

    q.prepare("SELECT * FROM Zakupy_klientow inner join Klienci_hurtowi"
              "ON Zakupy_klientow.Klienci_hurtowi_ID = Klienci_hurtowi.ID"
              "and  Klienci_hurtowi.Odbiorca = ?  and Klienci_hurtowi.Miasto = ? and "
              "Klienci_hurtowi.Ulica = ? and Klienci_hurtowi.Numer = ?");

    q.addBindValue(customer.name);
    q.addBindValue(customer.city);
    q.addBindValue(customer.street);
    q.addBindValue(customer.propertyNumber);

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

QStringList DataBaseApi::getProuducts()
{
    QStringList data;
    QSqlQuery   q;

    q.prepare("SELECT * FROM Produkty_na_stacji");

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

void DataBaseApi::addProduct(QString product)
{
    QSqlQuery q;

    q.prepare("INSERT or REPLACE INTO Produkty_na_stacji(Nazwa) values (?))");
    q.bindValue(0, product);

    q.exec();
    if (q.lastError().isValid())
    {
        qDebug() << q.lastError();
        throw q.lastError().text();
    }
}

} // namespace DataBaseApi
