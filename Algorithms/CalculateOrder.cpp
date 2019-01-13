#include "CalculateOrder.h"
#include <algorithm>
#include <QVector>

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

QVector<Common::DistancesStruct> CalculateOrder::getDistances()
{
    QVector<Common::DistancesStruct>       distances;
    const QVector<Common::DistancesStruct> allDistancesFromDatabase =
        mDatabaseApi.getAllDistances();
    QVector<Common::DistancesStruct> listOfClientsToGetDistanceFromGoogle;
    Common::DistancesStruct          tmp;

    QVector<Common::CustomerStruct> places;

    std::transform(mCustomers.begin(), mCustomers.end(), std::back_inserter(places),
                   [](const Common::OrdersStruct& it) { return it.customer; });

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

    distances.append(calculateAdditionalDistances(listOfClientsToGetDistanceFromGoogle));

    return distances;
}

QVector<Common::DistancesStruct> CalculateOrder::calculateAdditionalDistances(
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
            originString.replace(originString.size() - 1, 1, "&");
            destinationString.replace(destinationString.size() - 1, 1, "&");
            tmpDistances.append(
                getDistancesFromGoogle(originString, destinationString, howManyRequests));
            howManyRequests = 0;
            originString.clear();
            destinationString.clear();
        }
    }
    if (howManyRequests != 0)
    {
        originString.replace(originString.size() - 1, 1, "&");
        destinationString.replace(destinationString.size() - 1, 1, "&");
        tmpDistances.append(
            getDistancesFromGoogle(originString, destinationString, howManyRequests));
    }
    for (int i = 0; i < DistancesToCalculate.size(); i++)
    {
        tmpDistancesToCalculate.a    = DistancesToCalculate.at(i).a;
        tmpDistancesToCalculate.b    = DistancesToCalculate.at(i).b;
        tmpDistancesToCalculate.time = static_cast<uint>(tmpDistances.at(i));
        mDatabaseApi.addTravelTime(tmpDistancesToCalculate); // add distances to database
        allDistances.append(tmpDistancesToCalculate);
    }
    return allDistances;
}

QVector<QVector<Common::OrdersStruct>>
CalculateOrder::sortOrders(QVector<Common::DistancesStruct> distances)
{
    uint                       numberOfDraw = 10000;
    QVector<QVector<uint>>     returnVector;
    QPair<uint, QVector<uint>> startOrderAndTime(INT32_MAX, mIds);
    QPair<uint, QVector<uint>> bestOrderAndTime =
        optimalizeOrder(numberOfDraw, distances, startOrderAndTime);
    if (bestOrderAndTime.first < 8 * 60 * 60) // 8H  nie przekroczone, damy rade 1
    {
        returnVector.append(bestOrderAndTime.second);
    }
    else // liczymy 2 cieżarowki
    {
        const uint                                       numberOfDrawForTwoTrucks = 200;
        QPair<uint, QVector<uint>>                       bestOrderAndTime1;
        QPair<uint, QVector<uint>>                       bestOrderAndTime2;
        QPair<QVector<uint>, QVector<uint>>              dividedOrder;
        int                                              divider;
        QVector<uint>                                    tmpOrder = mIds;
        QPair<uint, QPair<QVector<uint>, QVector<uint>>> best;
        best.first = 1000000;
        for (uint i = 0; i < numberOfDrawForTwoTrucks; i++)
        {
            tmpOrder = drawOrder(tmpOrder);
            divider  = rand() % mIds.size() - 1 + 0;
            for (int j = 0; j < divider; j++)
                dividedOrder.first.append(tmpOrder);
            for (int k = divider; k < mIds.size(); k++)
                dividedOrder.second.append(tmpOrder);
            bestOrderAndTime1 = optimalizeOrder(
                numberOfDraw, distances, QPair<uint, QVector<uint>>(INT32_MAX, dividedOrder.first));
            if (bestOrderAndTime1.first > 8 * 60 * 60)
            {
                continue;
            }
            bestOrderAndTime2 =
                optimalizeOrder(numberOfDraw, distances,
                                QPair<uint, QVector<uint>>(INT32_MAX, dividedOrder.second));

            best = checkIfDrawIsBetterFor2(bestOrderAndTime1, bestOrderAndTime2, best);
        }
        returnVector.append(bestOrderAndTime1.second);
        returnVector.append(bestOrderAndTime2.second);
    }
    return transformIdsToOrders(returnVector);
}

QVector<QVector<Common::OrdersStruct>> CalculateOrder::getOrders()
{
    QVector<Common::DistancesStruct> Distances = getDistances();
    return sortOrders(Distances);
}

Common::CustomerStruct CalculateOrder::getCustomerById(uint id)
{
    if (id == 1)
    {
        return mDatabaseApi.getCustomerById(1); // Send base
    }
    QVector<Common::OrdersStruct>::const_iterator it = std::find_if(
        mCustomers.begin(), mCustomers.end(),
        [&id](const Common::OrdersStruct& customer) { return (customer.customer.id == id); });
    if (it != mCustomers.end())
        return it->customer;
    else
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
    qDebug() << "CalculateOrder::::Zawsze na razie przy URL go wyswietlimy, wtedy wiadomo czy nie "
                "jest zjebany i czy "
                "wgl sie Igiego metoda wywołała" +
                    URL + "\n\n\n\n";
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

QPair<uint, QVector<uint>>
CalculateOrder::optimalizeOrder(const uint numberOfDraw, QVector<Common::DistancesStruct> distances,
                                QPair<uint, QVector<uint>> startOrder)
{
    QPair<uint, QVector<uint>> best = startOrder;
    QPair<uint, QVector<uint>> tmp  = best;
    for (uint i = 0; i < numberOfDraw; i++)
    {
        tmp.second = drawOrder(tmp.second);
        tmp.first  = getFullTime(tmp.second, distances);
        best       = checkIfDrawIsBetter(best, tmp);
    }
    return best;
}

QPair<uint, QVector<uint>> CalculateOrder::checkIfDrawIsBetter(QPair<uint, QVector<uint>> best,
                                                               QPair<uint, QVector<uint>> tmp)
{
    if (tmp.first < best.first)
    {
        best = tmp;
    }
    return best;
}

QPair<uint, QPair<QVector<uint>, QVector<uint>>>
CalculateOrder::checkIfDrawIsBetterFor2(QPair<uint, QVector<uint>> bestOrderAndTime1,
                                        QPair<uint, QVector<uint>> bestOrderAndTime2,
                                        QPair<uint, QPair<QVector<uint>, QVector<uint>>> best)
{
    QPair<uint, QPair<QVector<uint>, QVector<uint>>> tmp;
    tmp.first         = bestOrderAndTime1.first + bestOrderAndTime2.first;
    tmp.second.first  = bestOrderAndTime1.second;
    tmp.second.second = bestOrderAndTime2.second;
    if (tmp.first < best.first)
    {
        best = tmp;
    }
    return best;
}

} // namespace Algorithms
