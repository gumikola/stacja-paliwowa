#include "CalculateOrder.h"
#include <algorithm>

namespace Algorithms {

CalculateOrder::CalculateOrder(DataBaseApi::DataBaseApi&          databaseApi,
                               const QList<Common::OrdersStruct>& customers)
    : mDatabaseApi(databaseApi)
    , mCustomers(customers)
{
    for (Common::OrdersStruct order : mCustomers)
    {
        mIds.append(order.customer.id);
    }
}

QList<Common::DistancesStruct> CalculateOrder::getDistances()
{
    QList<Common::DistancesStruct>       distances;
    const QList<Common::DistancesStruct> allDistancesFromDatabase =
        mDatabaseApi.getAllDistances().toList();
    QList<Common::DistancesStruct> listOfClientsToGetDistanceFromGoogle;
    Common::DistancesStruct        tmp;

    QList<Common::CustomerStruct> places;

    std::transform(mCustomers.begin(), mCustomers.end(), std::back_inserter(places),
                   [](const Common::OrdersStruct& it) { return it.customer; });

    places.append(mDatabaseApi.getCustomerById(Common::Benzol.id));

    for (int j = 0; j < places.size(); j++)
    {
        for (int k = j + 1; k < places.size(); k++)
        {
            QList<Common::DistancesStruct>::const_iterator it = std::find_if(
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

QList<Common::DistancesStruct> CalculateOrder::calculateAdditionalDistances(
    const QList<Common::DistancesStruct>& DistancesToCalculate)
{
    QList<Common::DistancesStruct> allDistances;
    Common::CustomerStruct         tmpCustomer;
    Common::DistancesStruct        tmpDistancesToCalculate;
    QString                        originString;
    QString                        destinationString;
    uint                           howManyRequests = 0;
    QList<int>                     tmpDistances;

    for (int i = 0; i < DistancesToCalculate.size(); i++)
    {
        if (howManyRequests < 8)
        {
            tmpCustomer = getCustomerById(DistancesToCalculate.at(i).a);
            originString.append(QString("%1_%2_%3|")
                                    .arg(tmpCustomer.city)
                                    .arg(tmpCustomer.street)
                                    .arg(tmpCustomer.propertyNumber));

            tmpCustomer = getCustomerById(DistancesToCalculate.at(i).b);

            destinationString.append(QString("%1_%2_%3|")
                                         .arg(tmpCustomer.city)
                                         .arg(tmpCustomer.street)
                                         .arg(tmpCustomer.propertyNumber));
            howManyRequests++;
        }
        else
        {
            originString.replace(originString.lastIndexOf("|"), 1, "&");
            destinationString.replace(originString.lastIndexOf("|"), 1, "&");
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
        mDatabaseApi.addTravelTime(tmpDistancesToCalculate);
        allDistances.append(tmpDistancesToCalculate);
    }
    return allDistances;
}

QList<QList<Common::OrdersStruct>>
CalculateOrder::sortOrders(QList<Common::DistancesStruct> distances)
{
    QList<QList<uint>>           returnVector;
    QPair<uint32_t, QList<uint>> startOrderAndTime(UINT32_MAX, mIds);
    QPair<uint32_t, QList<uint>> bestOrderAndTime = optimalizeOrder(distances);
    if (bestOrderAndTime.first < 8 * 60 * 60) // 8H  nie przekroczone, damy rade 1
    {
        returnVector.append(bestOrderAndTime.second);
    }
    else // liczymy 2 cieżarowki
    {
        const uint                                   numberOfDrawForTwoTrucks = 200;
        QPair<uint, QList<uint>>                     bestOrderAndTime1;
        QPair<uint, QList<uint>>                     bestOrderAndTime2;
        QPair<QList<uint>, QList<uint>>              dividedOrder;
        int                                          divider;
        QList<uint>                                  tmpOrder = mIds;
        QPair<uint, QPair<QList<uint>, QList<uint>>> best;
        best.first = 1000000;
        for (uint i = 0; i < numberOfDrawForTwoTrucks; i++)
        {
            tmpOrder = drawOrder(tmpOrder);
            qsrand(static_cast<uint>(QDateTime::currentMSecsSinceEpoch()));
            divider = qrand() % (mIds.size() - 1);
            for (int j = 0; j < divider; j++)
                dividedOrder.first.append(tmpOrder.at(j));
            for (int k = divider; k < mIds.size(); k++)
                dividedOrder.second.append(tmpOrder.at(k));
            bestOrderAndTime1 = optimalizeOrder(distances);
            if (bestOrderAndTime1.first > 8 * 60 * 60)
            {
                continue;
            }
            bestOrderAndTime2 = optimalizeOrder(distances);

            best = checkIfDrawIsBetterFor2(bestOrderAndTime1, bestOrderAndTime2, best);
        }
        returnVector.append(bestOrderAndTime1.second);
        returnVector.append(bestOrderAndTime2.second);
    }
    return transformIdsToOrders(returnVector);
}

QList<QList<Common::OrdersStruct>> CalculateOrder::getOrders()
{
    QList<Common::DistancesStruct> Distances = getDistances();
    return sortOrders(Distances);
}

Common::CustomerStruct CalculateOrder::getCustomerById(uint id)
{
    if (id == 1)
    {
        return mDatabaseApi.getCustomerById(1); // Send base
    }
    QList<Common::OrdersStruct>::const_iterator it = std::find_if(
        mCustomers.begin(), mCustomers.end(),
        [&id](const Common::OrdersStruct& customer) { return (customer.customer.id == id); });
    if (it != mCustomers.end())
        return it->customer;
    else
        throw QString(
            "Can't find Customer with ID: %1 in class CalculateOrder in method getCustonerById")
            .arg(id);
}

QList<int> CalculateOrder::getDistancesFromGoogle(const QString origin, const QString destination,
                                                  const uint size)
{
    QList<int> distances;
    QString    URL =
        QString("https://maps.googleapis.com/maps/api/distancematrix/"
                "json?origins=%1destinations=%2key=AIzaSyA6B9rzXYJzjFXloNN8bmmpHYA3Rl9UAdw")
            .arg(origin)
            .arg(destination);

    /// TO REMOVE BEFORE RELEASE
    ///
    qDebug() << "CalculateOrder::::Zawsze na razie przy URL go wyswietlimy, wtedy wiadomo czy nie "
                "jest zjebany i czy "
                "wgl sie Igiego metoda wywołała" +
                    URL + "\n\n\n\n";
    ///
    ///
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
                                             const QList<Common::DistancesStruct> distancesVector)
{
    for (Common::DistancesStruct distance : distancesVector)
    {
        if ((id1 == distance.a && id2 == distance.b) || (id1 == distance.b && id2 == distance.a))
        {
            return distance.time;
        }
    }
    throw QString("Can't find time beetwen two places!!!");
}

QList<uint> CalculateOrder::drawOrder(QList<uint>& order)
{

    std::random_shuffle(order.begin(), order.end());
    return order;
}

uint CalculateOrder::getFullTime(QList<uint>                          customers,
                                 const QList<Common::DistancesStruct> distancesVector)
{
    customers.push_back(Common::Benzol.id);
    customers.push_front(Common::Benzol.id);
    uint FullTime = 0;
    for (int i = 0; i < customers.size() - 1; i++)
    {
        FullTime +=
            (getTimeBeetwenTwoPlaces(customers.at(i), customers.at(i + 1), distancesVector));
    }
    return FullTime;
}

QList<QList<Common::OrdersStruct>> CalculateOrder::transformIdsToOrders(QList<QList<uint>> order)
{

    QList<QList<Common::OrdersStruct>> bestOrder;
    QList<Common::OrdersStruct>        oneTruckOrder;
    for (QList<uint> truck : order)
    {
        for (uint order : truck)
        {
            oneTruckOrder.append(getCustomerById(truck.at(static_cast<int>(order))).id);
        }
        bestOrder.append(oneTruckOrder);
    }
    return bestOrder;
}

QPair<uint, QList<uint>> CalculateOrder::optimalizeOrder(QList<Common::DistancesStruct> distances)
{
    QPair<uint, QList<uint>> best(UINT32_MAX, mIds);
    QPair<uint, QList<uint>> tmp          = best;
    const uint               numberOfDraw = 10000;
    for (uint i = 0; i < numberOfDraw; i++)
    {
        tmp.second = drawOrder(tmp.second);
        tmp.first  = getFullTime(tmp.second, distances);
        best       = checkIfDrawIsBetter(best, tmp);
    }
    return best;
}

QPair<uint, QList<uint>> CalculateOrder::checkIfDrawIsBetter(QPair<uint, QList<uint>> best,
                                                             QPair<uint, QList<uint>> tmp)
{
    if (tmp.first < best.first)
    {
        best = tmp;
    }
    return best;
}

QPair<uint, QPair<QList<uint>, QList<uint>>>
CalculateOrder::checkIfDrawIsBetterFor2(QPair<uint, QList<uint>> bestOrderAndTime1,
                                        QPair<uint, QList<uint>> bestOrderAndTime2,
                                        QPair<uint, QPair<QList<uint>, QList<uint>>> best)
{
    QPair<uint, QPair<QList<uint>, QList<uint>>> tmp;
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
