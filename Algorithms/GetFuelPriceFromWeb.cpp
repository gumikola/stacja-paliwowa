#include "GetFuelPriceFromWeb.h"

namespace Algorithms {

GetFuelPriceFromWeb::GetFuelPriceFromWeb()
{
    QNetworkRequest request;
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    QNetworkAccessManager manager;
    QNetworkReply*        response = manager.get(QNetworkRequest(QUrl(Common::URL)));
    QEventLoop            event;

    QObject::connect(response, SIGNAL(finished()), &event, SLOT(quit()));
    event.exec();
    mPageSourceCode = response->readAll();
}
double GetFuelPriceFromWeb::MatchRegex(QString Regex)
{
    QRegularExpression re(Regex);
    auto const         match = re.match(mPageSourceCode);
    double             price = match.captured(1).remove(1, 1).replace(",", ".").toDouble();
    return price;
}
double GetFuelPriceFromWeb::Get95Price()
{
    return MatchRegex("(?<=Pb95Price\">)(.*[0-9])(?=</span>&nbsp;)") / 1000;
}
double GetFuelPriceFromWeb::Get98Price()
{
    return MatchRegex("(?<=Pb98Price\">)(.*[0-9])(?=</span>&nbsp;)") / 1000;
}
double GetFuelPriceFromWeb::GetONPrice()
{
    return MatchRegex("(?<=EkodiselPrice\">)(.*[0-9])(?=</span>&nbsp;)") / 1000;
}
double GetFuelPriceFromWeb::GetOOPrice()
{
    return MatchRegex("(?<=EkotermPrice\">)(.*[0-9])(?=</span>&nbsp;)") / 1000;
}
} // namespace Algorithms
