#include "GoogleApi.h"
namespace Algorithms {
GoogleApi::GoogleApi(const Common::CustomerStruct& customer)
    : mCustomer(customer)
{
    QString URL = "https://maps.googleapis.com/maps/api/distancematrix/"
                  "json?origins=Benzol_Krolewiecka_1_Mysliborz"
                  "&destinations=" +
                  customer.street + "_" + customer.propertyNumber + "_" + customer.city +
                  "&key=AIzaSyA6B9rzXYJzjFXloNN8bmmpHYA3Rl9UAdw";
    /// TO REMOVE BEFORE RELEASE
    ///
    qDebug() << "GoogleApi::::Zawsze na razie przy URL go wyswietlimy, wtedy wiadomo czy nie jest "
                "zjebany i czy "
                "wgl sie Igiego metoda wywołała" +
                    URL + "\n\n\n\n";
    ///
    ///
    ///
    QNetworkRequest request;
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    QNetworkAccessManager manager;
    QNetworkReply*        response = manager.get(QNetworkRequest(QUrl(URL)));
    QEventLoop            event;
    QObject::connect(response, SIGNAL(finished()), &event, SLOT(quit()));
    event.exec();
    jsonResponse = response->readAll();
}
QString GoogleApi::GetDistanceFromJson()
{
    QJsonDocument response = QJsonDocument::fromJson(jsonResponse);
    CheckResponse(response);
    QString distance = response.object()
                           .value("rows")
                           .toArray()[0]
                           .toObject()
                           .value("elements")
                           .toArray()[0]
                           .toObject()
                           .value("distance")
                           .toObject()
                           .value("text")
                           .toString();

    QRegularExpression      re("\\d+");
    QRegularExpressionMatch match   = re.match(distance);
    QString                 matched = match.captured(0);
    return matched;
}
QString GoogleApi::GetTimeFromJson()
{
    QJsonDocument response = QJsonDocument::fromJson(jsonResponse);
    CheckResponse(response);
    int time = response.object()
                   .value("rows")
                   .toArray()[0]
                   .toObject()
                   .value("elements")
                   .toArray()[0]
                   .toObject()
                   .value("duration")
                   .toObject()
                   .value("value")
                   .toInt();

    return QString::number(time);
}
void GoogleApi::CheckResponse(QJsonDocument response)
{
    QString status = response.object().value("status").toString();
    if (status == "NOT_FOUND")
    {
        throw QString("Nie znaleziono lokalizacji");
    }
}
} // namespace Algorithms
