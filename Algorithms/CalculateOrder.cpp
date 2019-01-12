#include "CalculateOrder.h"
namespace Algorithms {

CalculateOrder::CalculateOrder(DataBaseApi::DataBaseApi&            databaseApi,
                               const QVector<Common::OrdersStruct>& customers)
    : mDatabaseApi(databaseApi)
    , mCustomers(customers)
{
}
// void CalculateOrder::GetCustomersID()
//{
//    for (int i = 0; i < mCustomers.size(); i++)
//    {
//        mIds.append(mDatabaseApi.getCustomerId(mCustomers.at(i).customer));
//    }
//}

bool CalculateOrder::checkIfDistanceIsInDatabase(
    const Common::DistancesStruct& distanceFromDatabase, const uint& a, const uint& b)
{
    if (((distanceFromDatabase.a == a) && distanceFromDatabase.b == b) ||
        ((distanceFromDatabase.a == b) && distanceFromDatabase.b == a))
        return true;
    else
        return false;
}

QVector<Common::DistancesStruct> CalculateOrder::GetDistances()
{
    QVector<Common::DistancesStruct>       Distances;
    const QVector<Common::DistancesStruct> allDistancesFromDatabase =
        mDatabaseApi.getAllDistances();
    QVector<Common::DistancesStruct> listOfClientsToGetDistanceFromGoogle;
    Common::DistancesStruct          tmp;
    for (int j = 0; j < mCustomers.size(); j++)
    {
        for (int k = j + 1; k < mCustomers.size(); k++) // zamiast mCustomer byly IDIKI
        {
            for (int i = 0; i < allDistancesFromDatabase.size(); i++)
            {
                if (checkIfDistanceIsInDatabase(allDistancesFromDatabase.at(i),
                                                static_cast<uint>(j), static_cast<uint>(k)))
                {
                    Distances.append(allDistancesFromDatabase.at(i));
                }
                else
                {
                    tmp.a = static_cast<uint>(j);
                    tmp.b = static_cast<uint>(k);
                    listOfClientsToGetDistanceFromGoogle.append(tmp);
                }
            }
        }
    }
    Distances.append(CalculateAdditionalDistances(listOfClientsToGetDistanceFromGoogle));

    return Distances;
}

QVector<Common::DistancesStruct> CalculateOrder::CalculateAdditionalDistances(
    const QVector<Common::DistancesStruct>& DistancesToCalculate)
{
    QVector<Common::DistancesStruct> allDistances;
    Common::CustomerStruct           tmpCustomer;
    Common::DistancesStruct          tmpDistancesToCalculate;
    QString                          originString;
    QString                          destinationString;
    uint                             howManyRequests = 0;
    QVector<int>                     tmpDistances;

    for (int i = 0; i < DistancesToCalculate.size(); i++)
    {
        if (howManyRequests < 8)
        {

            originString.clear();
            destinationString.clear();
            tmpCustomer = getCustomerById(DistancesToCalculate.at(i).a);
            originString.append(tmpCustomer.city + "_" + tmpCustomer.street + "_" +
                                tmpCustomer.propertyNumber + "|");
            tmpCustomer = getCustomerById(DistancesToCalculate.at(i).b);
            destinationString.append(tmpCustomer.city + "_" + tmpCustomer.street + "_" +
                                     tmpCustomer.propertyNumber + "|");
            howManyRequests++;
        }
        else
        {
            tmpDistances.append(
                getDistancesFromGoogle(originString, destinationString, howManyRequests));
            howManyRequests = 0;
        }
    }
    for (int i = 0; i < DistancesToCalculate.size(); i++)
    {
        tmpDistancesToCalculate.a    = DistancesToCalculate.at(i).a;
        tmpDistancesToCalculate.b    = DistancesToCalculate.at(i).b;
        tmpDistancesToCalculate.time = static_cast<uint>(tmpDistances.at(i));
        allDistances.append(tmpDistancesToCalculate);
    }
    return allDistances;
}

void CalculateOrder::SortOrders(QVector<Common::DistancesStruct>) {}

QVector<Common::OrdersStruct> CalculateOrder::GetOrders()
{
    QVector<Common::DistancesStruct> Distances = GetDistances();
    //    GetCustomersID();
    SortOrders(Distances);
    return mCustomers; // zeby warningiem
}

Common::CustomerStruct CalculateOrder::getCustomerById(uint id)
{
    if (id == 1)
    {
        /// wtedy jest benzolem
        /// return beznol;
    }
    else
    {
        for (int i = 0; i < mCustomers.size(); i++)
        {
            if (mCustomers.at(i).customer.id == id)
                return mCustomers.at(i).customer;
        }
    }
    throw QString(
        "Nie znaleziono Customera z tym ID w klasie CalculateOrder w metodzie getCustomerById");
}

QVector<int> CalculateOrder::getDistancesFromGoogle(const QString origin, const QString destination,
                                                    const uint size)
{
    QVector<int> distances;
    QString      URL = "https://maps.googleapis.com/maps/api/distancematrix/"
                  "json?origins=" +
                  origin + "destinations=" + destination +
                  "key=AIzaSyA6B9rzXYJzjFXloNN8bmmpHYA3Rl9UAdw";
    QNetworkRequest request;
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    QNetworkAccessManager manager;
    QNetworkReply*        response = manager.get(QNetworkRequest(QUrl(URL)));
    QEventLoop            event;
    QObject::connect(response, SIGNAL(finished()), &event, SLOT(quit()));
    event.exec();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response->readAll());
    for (int i = 0; i < static_cast<int>(size); i++)
    {
        distances.append(jsonResponse.object()
                             .value("rows")
                             .toArray()[0]
                             .toObject()
                             .value("elements")
                             .toArray()[i]
                             .toObject()
                             .value("duration")
                             .toObject()
                             .value("value")
                             .toInt());
    }
    return distances;
}
} // namespace Algorithms
