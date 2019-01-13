#include "CalculateRoute.h"
namespace Algorithms {
CalculateRoute::CalculateRoute(const QVector<Common::OrdersStruct> Customers)
    : mCustomers(Customers)
{
    QString CustomersStr;
    for (int i = 0; i < Customers.size(); i++)
    {
        CustomersStr = CustomersStr + mCustomers[i].customer.street + "_" +
                       mCustomers[i].customer.propertyNumber + "_" + mCustomers[i].customer.city;
        if (i != mCustomers.size() - 1)
            CustomersStr = CustomersStr + "|";
        else
            CustomersStr = CustomersStr + "&";
    }
    QString URL = "https://maps.googleapis.com/maps/api/directions/"
                  "json?origin=Cynamonowa_15_Wrocław"
                  "&destination=Bacciarellego_40D_Wrocław&waypoints=optimize:true|" +
                  CustomersStr + "key=AIzaSyA6B9rzXYJzjFXloNN8bmmpHYA3Rl9UAdw";
    qDebug() << "CalculateRoute::::Zawsze na razie przy URL go wyswietlimy, wtedy wiadomo czy nie "
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
    jsonResponse = response->readAll();
}
QVector<Common::OrdersStruct> CalculateRoute::CalculateOrder()
{
    QVector<Common::OrdersStruct> ReturnStruct;
    QJsonDocument                 response = QJsonDocument::fromJson(jsonResponse);
    QJsonArray                    Order =
        response.object().value("routes").toArray()[0].toObject().value("waypoint_order").toArray();

    for (int i = 0; i < Order.size(); i++)
    {
        for (int j = 0; j < Order.size(); j++)
        {
            if (Order[i].toInt() == j)
            {
                ReturnStruct.push_back(mCustomers[j]);
            }
        }
    }
    qDebug() << Order << endl;
    return ReturnStruct;
}
} // namespace Algorithms
