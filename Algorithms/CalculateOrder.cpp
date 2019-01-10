#include "CalculateOrder.h"
namespace Algorithms {
CalculateOrder::CalculateOrder(const QVector<Common::OrdersStruct> Customers)
    : mCustomers(Customers)
{
}
QVector<int> CalculateOrder::GetDistanceVector(QVector<Common::CustomerStruct> customers,
                                               int                             CustomerID)
{
    QString originString = "";
    for (int i = 0; i < customers.size(); i++)
    {
        originString.append(customers[CustomerID].city + "_" + customers[CustomerID].street + "_" +
                            customers[CustomerID].propertyNumber);
        if (i == customers.size() - 1)
            originString.append("&");
        else
            originString.append("|");
    }
    QString destinationString = "";
    for (int i = 0; i < customers.size(); i++)
    {
        destinationString.append(customers[i].city + "_" + customers[i].street + "_" +
                                 customers[i].propertyNumber);
        if (i == customers.size() - 1)
            destinationString.append("&");
        else
            destinationString.append("|");
    }
    QString URL = "https://maps.googleapis.com/maps/api/distancematrix/"
                  "json?origins=" +
                  originString + "destinations=" + destinationString +
                  "key=AIzaSyA6B9rzXYJzjFXloNN8bmmpHYA3Rl9UAdw";
    QNetworkRequest request;
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    QNetworkAccessManager manager;
    QNetworkReply*        response = manager.get(QNetworkRequest(QUrl(URL)));
    QEventLoop            event;
    QObject::connect(response, SIGNAL(finished()), &event, SLOT(quit()));
    event.exec();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response->readAll());
    QVector<int>  distances;
    for (int i = 0; i < customers.size(); i++)
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

QVector<QVector<Common::OrdersStruct>> CalculateOrder::GetOrder()
{
    QVector<QVector<Common::OrdersStruct>> returnVector;
    QVector<Common::OrdersStruct>          returnStruct;

    QPair<QVector<CalculateOrder::CustomerDistanceID>, int> order =
        CalculateOrderFor2Or1Trucks(false);
    if (order.second < 6 * 60 * 60)
    {
        for (int i = 0; i < order.first.size(); i++)
        {
            for (int j = 0; j < mCustomers.size(); j++)
            {
                if (mCustomers[j].customer.name == order.first[i].Customer.name)
                    returnStruct.append(mCustomers[j]);
            }
        }
        returnVector.append(returnStruct);
        returnStruct.clear();
    }
    else // jak trasa przekorczy 8 h
    {
        order.second = 0;
        order        = CalculateOrderFor2Or1Trucks(true);
        returnVector.clear();
        int i = 1;
        while (!order.first[i].isHome)
        {
            for (int j = 0; j < mCustomers.size(); j++)
            {
                if (mCustomers[j].customer.name == order.first[i].Customer.name)
                    returnStruct.append(mCustomers[j]);
            }
            i++;
        }
        returnVector.append(returnStruct);
        returnStruct.clear();
        while (i < order.first.size())
        {
            for (int j = 0; j < mCustomers.size(); j++)
            {
                if (mCustomers[j].customer.name == order.first[i].Customer.name)
                    returnStruct.append(mCustomers[j]);
            }
            i++;
        }
        returnVector.append(returnStruct);
    }

    return returnVector;
}
QVector<CalculateOrder::CustomerDistanceID>
CalculateOrder::drawPosition(QVector<CalculateOrder::CustomerDistanceID> Customers)
{
    int firstElement = 0, secondElement = 0;

    while (firstElement == secondElement)
    {
        firstElement = rand() % (Customers.size() - 2) + 1;
        if (Customers[firstElement].isHome)
        {
            secondElement = rand() % (Customers.size() - 4) + 2;
        }
        else
            secondElement = rand() % (Customers.size() - 2) + 1;
    }
    Customers.move(firstElement, secondElement);
    return Customers;
}
QPair<int, int> CalculateOrder::CalculateTime(QVector<CalculateOrder::CustomerDistanceID> Order,
                                              bool                                        twoTrucks)
{
    QPair<int, int> totalTime;
    totalTime.first  = 0;
    totalTime.second = 0;
    if (twoTrucks)
    {
        int i = 0;
        while (1)
        {
            if (Order[i].isHome && i != 0)
                break;
            totalTime.first += Order[i].distancesTable[Order[i + 1].ID];
            i++;
        }
        while (i < Order.size() - 1)
        {
            totalTime.second += Order[i].distancesTable[Order[i + 1].ID];
            i++;
        }
        //        totalTime = qMax(totalTime1, totalTime2);
    }
    else
    {
        for (int i = 0; i < Order.size() - 1; i++)
            totalTime.first += Order[i].distancesTable[Order[i + 1].ID];
    }

    return totalTime;
}
QPair<QVector<CalculateOrder::CustomerDistanceID>, int>
CalculateOrder::CalculateOrderFor2Or1Trucks(bool twoTrucks)
{
    Common::CustomerStruct                 home;
    QVector<QVector<Common::OrdersStruct>> returnVector;
    QVector<Common::OrdersStruct>          returnStruct;
    home.city           = "Mysliborz";
    home.street         = "Krolewiecka";
    home.propertyNumber = "1";
    home.name           = "Benzol";
    QVector<Common::CustomerStruct> originalOrder;
    for (int i = 0; i < mCustomers.size(); i++)
        originalOrder.append(mCustomers[i].customer);
    originalOrder.push_front(home);
    originalOrder.push_back(home);
    QVector<Common::CustomerStruct>             order = originalOrder;
    QVector<CalculateOrder::CustomerDistanceID> Customers;
    CalculateOrder::CustomerDistanceID          cus;
    if (twoTrucks)
        order.insert(order.size() / 2, home);
    for (int i = 0; i < order.size(); i++)
    {
        cus.Customer       = order[i];
        cus.ID             = i;
        cus.distancesTable = GetDistanceVector(order, i);
        if (order[i].name == "Benzol")
            cus.isHome = true;
        else
            cus.isHome = false;
        Customers.append(cus);
    }

    int testDuration =
        CalculateTime(Customers, twoTrucks).first + CalculateTime(Customers, twoTrucks).second;
    int bestDuration = testDuration;

    QVector<CalculateOrder::CustomerDistanceID> bestOrder = Customers;
    QVector<CalculateOrder::CustomerDistanceID> testOrder;
    for (int i = 0; i < 1000; i++)
    {
        testOrder = drawPosition(Customers);
        testDuration =
            CalculateTime(testOrder, twoTrucks).first + CalculateTime(testOrder, twoTrucks).second;
        if (testDuration < bestDuration)
        {
            bestOrder    = testOrder;
            bestDuration = testDuration;
        }
    }
    QPair<QVector<CalculateOrder::CustomerDistanceID>, int> returnPair;
    returnPair.first = bestOrder;
    returnPair.second =
        qMax(CalculateTime(bestOrder, twoTrucks).first, CalculateTime(bestOrder, twoTrucks).second);
    qDebug() << returnPair.second;
    return returnPair;
}

} // namespace Algorithms
