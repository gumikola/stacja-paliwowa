#include "CalculateOrder.h"
#include <algorithm>

namespace Algorithms {

CalculateOrder::CalculateOrder(DataBaseApi::DataBaseApi&            databaseApi,
                               const QVector<Common::OrdersStruct>& customers)
    : mDatabaseApi(databaseApi)
    , mCustomers(customers)
{
    for (int i = 0; i < customers.size(); i++)
    {
        mIds.append(customers.at(i).customer.id);
    }
}

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
    QVector<Common::DistancesStruct>       distances;
    const QVector<Common::DistancesStruct> allDistancesFromDatabase =
        mDatabaseApi.getAllDistances();
    QVector<Common::DistancesStruct> listOfClientsToGetDistanceFromGoogle;
    Common::DistancesStruct          tmp;

    QVector<Common::CustomerStruct> places;

    for (Common::OrdersStruct order : mCustomers)
    {
        places.push_back(order.customer);
    }
    //    std::transform(mCustomers.begin(), mCustomers.end(), std::back_inserter(places),
    //                   [](QVector<Common::OrdersStruct>::iterator it) { return it->customer; });

    places.append(mDatabaseApi.getCustomerById(1));

    for (int j = 0; j < places.size(); j++)
    {
        for (int k = j + 1; k < places.size(); k++)
        {
            QVector<Common::DistancesStruct>::const_iterator it = std::find_if(
                allDistancesFromDatabase.cbegin(), allDistancesFromDatabase.cend(),
                [&places, j, k](const Common::DistancesStruct& distance) {
                    return ((distance.a == places.at(k).id) and (distance.b == places.at(j).id)) or
                           ((distance.a == places.at(j).id) and (distance.b == places.at(k).id));
                });
            if (it != allDistancesFromDatabase.cend())
            {
                distances.push_back(*it);
            }
            else
            {
                listOfClientsToGetDistanceFromGoogle.push_back(
                    Common::DistancesStruct(places.at(j).id, places.at(k).id));
            }
        }
    }

    distances.append(CalculateAdditionalDistances(listOfClientsToGetDistanceFromGoogle));

    return distances;
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

QVector<QVector<Common::OrdersStruct>>
CalculateOrder::SortOrders(QVector<Common::DistancesStruct> distances)
{
    const int              numberOfDraw = 10000;
    uint                   bestTime     = INT32_MAX;
    uint                   tmpTime;
    QVector<uint>          tmpOrder = mIds;
    QVector<uint>          bestOrder;
    QVector<QVector<uint>> returnVector;
    for (int i = 0; i < numberOfDraw; i++)
    {
        tmpOrder = drawOrder(tmpOrder);
        tmpTime  = getFullTime(tmpOrder, distances);
        if (tmpTime < bestTime)
        {
            bestTime  = tmpTime;
            bestOrder = tmpOrder;
        }
    }
    if (bestTime < 8 * 60 * 60) // 8H  nie przekroczone liczymy
    {
        returnVector.append(bestOrder);
    }
    else // liczymy 2 cieżarowki
    {
        QVector<uint> tmpOrder1;
        QVector<uint> tmpOrder2;
        QVector<uint> bestOrder1;
        QVector<uint> bestOrder2;
        int           divider;
        uint          tmpTime1 = 0;
        uint          tmpTime2 = 0;
        bestTime               = INT32_MAX;
        for (int i = 0; i < numberOfDraw; i++)
        {

            tmpOrder = drawOrder(tmpOrder);
            divider  = rand() % mIds.size() - 1 + 0;
            for (int j = 0; j < divider; j++)
                tmpOrder1.append(tmpOrder.at(j));

            for (int k = divider; k < tmpOrder.size(); k++)
                tmpOrder2.append(tmpOrder.at(k));
            tmpTime1 = getFullTime(tmpOrder1, distances);
            tmpTime2 = getFullTime(tmpOrder2, distances);
            if (tmpTime1 < 8 * 60 * 60 || tmpTime2 < 8 * 60 * 60)
            {
                tmpTime = tmpTime1 + tmpTime2;
                if (tmpTime < bestTime)
                {
                    bestTime   = tmpTime;
                    bestOrder1 = tmpOrder1;
                    bestOrder2 = tmpOrder2;
                }
            }
        }
        returnVector.append(bestOrder1);
        returnVector.append(bestOrder2);
    }
    return transformIdsToOrders(returnVector);
}

QVector<QVector<Common::OrdersStruct>> CalculateOrder::GetOrders()
{
    QVector<Common::DistancesStruct> Distances = GetDistances();
    return SortOrders(Distances);
}

Common::CustomerStruct CalculateOrder::getCustomerById(uint id)
{
    for (int i = 0; i < mCustomers.size(); i++)
    {
        if (mCustomers.at(i).customer.id == id)
            return mCustomers.at(i).customer;
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

uint CalculateOrder::getTimeBeetwenTwoPlaces(uint id1, uint id2,
                                             const QVector<Common::DistancesStruct> distancesVector)
{
    for (int i = 0; i < distancesVector.size(); i++)
    {
        if ((id1 == distancesVector.at(i).a && id2 == distancesVector.at(i).b) ||
            (id1 == distancesVector.at(i).b && id2 == distancesVector.at(i).a))
        {
            return distancesVector.at(i).time;
        }
    }
    throw QString("Nie znaleziono czasu pomiedzy dwoma miejscami!!!");
}

QVector<uint> CalculateOrder::drawOrder(QVector<uint> Order)
{
    QList<uint> listOrder = Order.toList();
    std::random_shuffle(listOrder.begin(), listOrder.end());
    return listOrder.toVector();
}

uint CalculateOrder::getFullTime(QVector<uint>                          customers,
                                 const QVector<Common::DistancesStruct> distancesVector)
{
    customers.push_back(1); // add benzol
    customers.push_front(1);
    uint FullTime = 0;
    for (int i = 0; i < customers.size() - 1; i++)
    {
        FullTime +=
            (getTimeBeetwenTwoPlaces(customers.at(i), customers.at(i + 1), distancesVector));
    }
    return FullTime;
}

QVector<QVector<Common::OrdersStruct>>
CalculateOrder::transformIdsToOrders(QVector<QVector<uint>> bestOrder)
{
    QVector<QVector<Common::OrdersStruct>> best;
    QVector<Common::OrdersStruct>          oneTruckOrder;
    for (int i = 0; i < bestOrder.size(); i++)
    {
        for (int j = 0; j < bestOrder.at(i).size(); j++)
        {
            oneTruckOrder.append(getCustomerById(bestOrder.at(i).at(j)).id);
        }
        best.append(oneTruckOrder);
    }
    return best;
}
} // namespace Algorithms
