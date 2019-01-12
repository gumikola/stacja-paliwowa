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

bool CalculateOrder::checkIfDistanceIsInDatabase(const Common::Distances& distanceFromDatabase,
                                                 const uint& a, const uint& b)
{
    if (((distanceFromDatabase.a == a) && distanceFromDatabase.b == b) ||
        ((distanceFromDatabase.a == b) && distanceFromDatabase.b == a))
        return true;
    else
        return false;
}

QVector<Common::Distances> CalculateOrder::GetDistances()
{
    QVector<Common::Distances>       Distances;
    const QVector<Common::Distances> allDistancesFromDatabase = mDatabaseApi.getAllDistances();
    QVector<Common::Distances>       listOfClientsToGetDistanceFromGoogle;

    Common::Distances tmp;
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
        tmp.a = 1; // benzol id in database in table distances
        tmp.b =
            mCustomers.at(j).customer.id; // add Benzol to distances, which is for sure in database
        Distances.append(tmp);
    }
    Distances.append(CalculateAdditionalDistances(listOfClientsToGetDistanceFromGoogle));

    return Distances;
}

QVector<Common::Distances>
CalculateOrder::CalculateAdditionalDistances(const QVector<Common::Distances>& DistancesToCalculate)
{

    //    /// TODO
    //    QVector<QVector<Common::CustomerStruct>> originPlaces;
    //    QVector<QVector<Common::CustomerStruct>> destinationPlaces;
    //    QVector<QString>                         originString;
    //    QVector<QString>                         destinationString;
    //    int                                      howManyRequests = 0;
    //    Common::CustomerStruct                   tmpCustomer;
    //    QVector<Common::Distances>               Distances;
    //    Common::Distances                        tmpDistance;
    //    for (int i = 0; i < DistancesToCalculate.size(); i++)
    //    {
    //        if (originPlaces.at(howManyRequests).size() < 9)
    //        {
    //            tmpCustomer = getCustomerById(DistancesToCalculate.at(i).a);
    //            originString.append(tmpCustomer.city + "_" + tmpCustomer.street + "_" +
    //                                tmpCustomer.propertyNumber + "|");
    //            tmpCustomer = getCustomerById(DistancesToCalculate.at(i).b);
    //            destinationString.append(tmpCustomer.city + "_" + tmpCustomer.street + "_" +
    //                                     tmpCustomer.propertyNumber + "|");

    //            howManyRequests++;
    //            //
    //            /// TODO ogarnac "|" na końcu stringów
    //            //
    //        }
    //    }
    //    return Distances; // zeby warningiem nie sral
}

void CalculateOrder::SortOrders(QVector<Common::Distances>)
{
    /// TODO ;
}

QVector<Common::OrdersStruct> CalculateOrder::GetOrders()
{
    QVector<Common::Distances> Distances = GetDistances();
    //    GetCustomersID();
    SortOrders(Distances);
    return mCustomers; // zeby warningiem nie srał
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
