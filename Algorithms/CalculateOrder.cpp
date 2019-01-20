#include "CalculateOrder.h"
#include <algorithm>
#include <QMessageBox>

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
    qDebug() << __PRETTY_FUNCTION__;
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
    const QList<Common::DistancesStruct>& distancesToCalculate)
{
    qDebug() << __PRETTY_FUNCTION__;
    QList<Common::DistancesStruct> allDistances;
    Common::CustomerStruct         tmpCustomer;
    Common::DistancesStruct        tmpDistancesToCalculate;
    QString                        originString;
    QString                        destinationString;
    uint                           howManyRequests = 0;
    QList<int>                     tmpDistances;

    for (int i = 0; i < distancesToCalculate.size(); i++)
    {

        tmpCustomer = getCustomerById(distancesToCalculate.at(i).a);
        originString.append(QString("%1_%2_%3|")
                                .arg(tmpCustomer.city)
                                .arg(tmpCustomer.street)
                                .arg(tmpCustomer.propertyNumber));

        tmpCustomer = getCustomerById(distancesToCalculate.at(i).b);

        destinationString.append(QString("%1_%2_%3|")
                                     .arg(tmpCustomer.city)
                                     .arg(tmpCustomer.street)
                                     .arg(tmpCustomer.propertyNumber));
        howManyRequests++;

        if (howManyRequests > 7)
        {
            originString.replace(originString.lastIndexOf("|"), 1, "&");
            destinationString.replace(destinationString.lastIndexOf("|"), 1, "&");
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
    for (int i = 0; i < distancesToCalculate.size(); i++)
    {
        tmpDistancesToCalculate.a    = distancesToCalculate.at(i).a;
        tmpDistancesToCalculate.b    = distancesToCalculate.at(i).b;
        tmpDistancesToCalculate.time = static_cast<uint>(tmpDistances.at(i));
        mDatabaseApi.addTravelTime(tmpDistancesToCalculate);
        allDistances.append(tmpDistancesToCalculate);
    }
    return allDistances;
}

QList<QList<Common::OrdersStruct>>
CalculateOrder::sortOrders(QList<Common::DistancesStruct> distances)
{
    qDebug() << __PRETTY_FUNCTION__;
    QList<QList<uint>>           returnList;
    QPair<uint32_t, QList<uint>> startOrderAndTime(UINT32_MAX, mIds);
    QPair<uint32_t, QList<uint>> bestOrderAndTime = optimalizeOrder(distances, mIds);
    qDebug() << "Najlepszy czas: " + QString::number(bestOrderAndTime.first);
    if (bestOrderAndTime.first < 8 * 60 * 60) // 8H  nie przekroczone, damy rade 1
    {
        returnList.append(bestOrderAndTime.second);
    }
    else // liczymy 2 cieżarowki
    {

        startOrderAndTime.first = 1000000;
        startOrderAndTime.second.append(Common::Benzol.id);
        bestOrderAndTime = optimalizeOrder(distances, startOrderAndTime.second);
        returnList       = divideOrderFor2Trucks(bestOrderAndTime);
    }
    return transformIdsToOrders(returnList);
}

QList<QList<Common::OrdersStruct>> CalculateOrder::getOrders()
{
    qDebug() << __PRETTY_FUNCTION__;
    QList<Common::DistancesStruct> Distances = getDistances();
    return sortOrders(Distances);
}

Common::CustomerStruct CalculateOrder::getCustomerById(uint id)
{
    qDebug() << __PRETTY_FUNCTION__;
    if (id == Common::Benzol.id)
    {
        return mDatabaseApi.getCustomerById(Common::Benzol.id); // Send base
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

Common::OrdersStruct CalculateOrder::getOrderByCustomerId(uint id)
{
    QList<Common::OrdersStruct>::const_iterator it = std::find_if(
        mCustomers.begin(), mCustomers.end(),
        [&id](const Common::OrdersStruct& order) { return (order.customer.id == id); });
    if (it != mCustomers.end())
        return *it;
    else
        throw QString(
            "Can't find Customer with ID: %1 in class CalculateOrder in method getCustonerById")
            .arg(id);
}

QList<int> CalculateOrder::getDistancesFromGoogle(const QString origin, const QString destination,
                                                  const uint size)
{
    qDebug() << __PRETTY_FUNCTION__;
    QList<int> distances;
    QString    URL =
        QString("https://maps.googleapis.com/maps/api/distancematrix/"
                "json?origins=%1destinations=%2key=AIzaSyA6B9rzXYJzjFXloNN8bmmpHYA3Rl9UAdw")
            .arg(origin)
            .arg(destination);
    QNetworkRequest request;
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    QNetworkAccessManager manager;
    QNetworkReply*        response = manager.get(QNetworkRequest(QUrl(URL)));
    QEventLoop            event;
    QObject::connect(response, SIGNAL(finished()), &event, SLOT(quit()));
    event.exec();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response->readAll());
    qDebug() << jsonResponse;
    for (int i = 0; i < static_cast<int>(size); i++)
    {
        distances.append(jsonResponse.object()
                             .value("rows")
                             .toArray()[i]
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

bool CalculateOrder::checkIfBenzolIsInOrder(QList<uint>& order)
{
    for (int i = 0; i < order.size(); i++)
    {
        if (order.at(i) == Common::Benzol.id)
        {
            return true;
        }
    }
    return false;
}
QList<uint> CalculateOrder::drawOrder(QList<uint>& order)
{
    if (checkIfBenzolIsInOrder(order))
    {

        bool isBenzolInside = false;

        while (!isBenzolInside)
        {
            std::random_shuffle(order.begin(), order.end());
            for (int i = 0; i < order.size(); i++)
            {
                if (order.at(i) == Common::Benzol.id && i < (order.size() - 1) && i > 0)
                {
                    isBenzolInside = true;
                }
            }
        }
    }
    else
    {
        std::random_shuffle(order.begin(), order.end());
    }
    return order;
}

uint CalculateOrder::getFullTime(QList<uint>                          customers,
                                 const QList<Common::DistancesStruct> distancesVector)
{
    customers.push_back(Common::Benzol.id);
    customers.push_front(Common::Benzol.id);
    QPair<uint, uint> fullTime(0, 0);
    int               i = 0;
    while (i < customers.size() - 1)
    {
        if (i != 0 && customers.at(i) == Common::Benzol.id)
            break;
        fullTime.first +=
            (getTimeBeetwenTwoPlaces(customers.at(i), customers.at(i + 1), distancesVector));
        i++;
    }
    if (i < customers.size() - 2)
    {
        while (i < customers.size() - 1)
        {
            fullTime.second +=
                (getTimeBeetwenTwoPlaces(customers.at(i), customers.at(i + 1), distancesVector));
            i++;
        }
        if (fullTime.first > 8 * 60 * 60 || fullTime.second > 8 * 60 * 60)
            return 1000000; // if we have 2 trucks and some of them need to drive more than 8 h
                            // return big value to make algorithm think that's bad idea.
    }
    return fullTime.first + fullTime.second;
}

QList<QList<Common::OrdersStruct>> CalculateOrder::transformIdsToOrders(QList<QList<uint>> order)
{
    qDebug() << __PRETTY_FUNCTION__;
    QList<QList<Common::OrdersStruct>> bestOrder;
    QList<Common::OrdersStruct>        oneTruckOrder;

    for (int i = 0; i < order.size(); i++)
    {
        for (int j = 0; j < order.at(i).size(); j++)
        {
            oneTruckOrder.append(getOrderByCustomerId(order.at(i).at(j)));
        }
        bestOrder.append(oneTruckOrder);
        oneTruckOrder.clear();
    }
    return bestOrder;
}

QPair<uint, QList<uint>> CalculateOrder::optimalizeOrder(QList<Common::DistancesStruct> distances,
                                                         QList<uint>                    order)
{
    qDebug() << __PRETTY_FUNCTION__;
    QPair<uint, QList<uint>> best(UINT32_MAX, order);
    QPair<uint, QList<uint>> tmp          = best;
    const uint               numberOfDraw = 100000;
    for (uint i = 0; i < numberOfDraw; i++)
    {
        tmp.second = drawOrder(tmp.second);
        tmp.first  = getFullTime(tmp.second, distances);
        best       = checkIfDrawIsBetter(best, tmp);
    }
    qDebug() << best;
    return best;
}

QPair<uint, QList<uint>> CalculateOrder::checkIfDrawIsBetter(QPair<uint, QList<uint>> best,
                                                             QPair<uint, QList<uint>> tmp)
{
    //    qDebug() << __PRETTY_FUNCTION__;
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
    //    qDebug() << __PRETTY_FUNCTION__;
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

QList<QList<uint>>
CalculateOrder::divideOrderFor2Trucks(QPair<uint32_t, QList<uint>> bestOrderAndTime)
{
    //    qDebug() << __PRETTY_FUNCTION__;
    QList<QList<uint>> returnList;
    QList<uint>        OrderForOneTruck;
    int                i = 0;
    while (i < bestOrderAndTime.second.size())
    {
        if (bestOrderAndTime.second.at(i) == Common::Benzol.id)
        {
            break;
        }
        OrderForOneTruck.append(bestOrderAndTime.second.at(i));
        i++;
    }
    returnList.append(OrderForOneTruck);
    OrderForOneTruck.clear();
    for (int j = i + 1; j < bestOrderAndTime.second.size(); j++)
    {
        OrderForOneTruck.append(bestOrderAndTime.second.at(j));
    }
    returnList.append(OrderForOneTruck);
    return returnList;
}

} // namespace Algorithms
