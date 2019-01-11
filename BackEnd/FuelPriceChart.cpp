#include "FuelPriceChart.h"
#include <QDate>
#include <QDateTime>
#include <QFile>
#include <QRandomGenerator>
#include <QSplineSeries>
#include <QStringList>
#include <QTextStream>
#include <QtCharts>

namespace BackEnd {

FuelPriceChart::FuelPriceChart(Ui::MainWindow* ui, DataBaseApi::DataBaseApi& databaseApi)
    : mDatabaseApi(databaseApi)
    , mUi(ui)
    , mFuelPriceChartChoosedFuelType(*ui->FuelPriceTabFuelType)
    , mSeries(new QPair<QSplineSeries, QSplineSeries>)
    , mChart(new QChart)
    , mAxisX(new QDateTimeAxis)
    , mAxisY(new QValueAxis)
    , mChartView(new QChartView(mChart))
{
    connect(ui->FuelPriceTabFuelType, SIGNAL(activated(int)), this,
            SLOT(choosedFuelTypeChanged(int)));

    QGridLayout layout;
    layout.addWidget(mChartView);
    mUi->FuelPriceTabChartLayout->addLayout(&layout);
}

void FuelPriceChart::removeAndCreateNew() {}

void FuelPriceChart::choosedFuelTypeChanged(int index)
{
    qDebug("choosedFuelTypeChanged to: %d", index);
    if (mFuelPriceChartChoosedFuelType.currentIndex() <= 0)
        return;

    mChart->removeSeries(&mSeries->first);
    mChart->removeSeries(&mSeries->second);

    delete mSeries;
    mSeries = new QPair<QSplineSeries, QSplineSeries>;

    fillSeries(static_cast<Common::FuelType>(index - 1));

    qDebug("first size = %d, second size = %d", mSeries->first.count(), mSeries->second.count());

    mSeries->first.setPen(QPen(QColor(Qt::GlobalColor::blue)));
    mSeries->second.setPen(QPen(QColor(Qt::GlobalColor::red)));

    setUpChart();
    setUpXAxis();
    setUpYAxis();
    setUpChartView();
}

void FuelPriceChart::setUpXAxis()
{
    mSeries->first.detachAxis(mAxisX);
    mSeries->second.detachAxis(mAxisX);
    mChart->removeAxis(mAxisX);

    delete mAxisX;
    mAxisX = new QDateTimeAxis;

    mAxisX->setFormat("dd MMM yyyy");
    mAxisX->setLabelsAngle(90);
    mAxisX->setTitleText("Data (dd mm rrrr)");
    qDebug() << "Set range   " << mDateTimeStart.date() << mDateTimeEnd.date()
             << "ticks: " << mSize / 2;
    mAxisX->setTickCount(mSize / 2);
    mAxisX->setRange(mDateTimeStart, mDateTimeEnd);
    mChart->addAxis(mAxisX, Qt::AlignBottom);
    mSeries->first.attachAxis(mAxisX);
    mSeries->second.attachAxis(mAxisX);
}

void FuelPriceChart::setUpYAxis()
{
    mSeries->first.detachAxis(mAxisY);
    mSeries->second.detachAxis(mAxisY);
    mChart->removeAxis(mAxisY);

    delete mAxisY;
    mAxisY = new QValueAxis;

    mAxisY->setLabelFormat("%.2f");
    mAxisY->setTickCount(15);
    mAxisY->setTitleText("Cena paliwa");
    mAxisY->setRange(mMin - 50, mMax + 50);
    mChart->addAxis(mAxisY, Qt::AlignLeft);

    mSeries->first.attachAxis(mAxisY);
    mSeries->second.attachAxis(mAxisY);
}

void FuelPriceChart::setUpChartView()
{
    mChartView->setRenderHint(QPainter::Antialiasing);
    mChartView->resize(mUi->FuelPriceTabChartLayout->geometry().width(),
                       mUi->FuelPriceTabChartLayout->geometry().height());
}

void FuelPriceChart::setUpChart()
{
    mChart->addSeries(&mSeries->first);
    mChart->addSeries(&mSeries->second);
    mChart->legend()->hide();
    mChart->setTitle(QString("Wykres cen paliwa typu: ") +
                     mFuelPriceChartChoosedFuelType.currentText());
}

void FuelPriceChart::fillSeries(Common::FuelType fuelType)
{
    mSeries->first.clear();
    mSeries->second.clear();

    QFile file(getChartDataFileName(fuelType));

    if (not file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw QString("BackEnd::FuelPriceChart::fillSeries cannot open file: ") +
            getChartDataFileName(fuelType);

    QTextStream fileStream(&file);
    bool        once = true;

    QDateTime dateTime;
    mSize = 0;
    mMin  = 20000;
    mMax  = 0;

    while (!fileStream.atEnd())
    {
        mSize++;
        if (mSize > 60)
            break;

        QString     line = fileStream.readLine();
        QStringList list = line.split(",");

        dateTime.setDate(QDate::fromString(list.first(), "dd-MM-yyyy"));
        qDebug() << list.first() << list.last() << dateTime.date() << "   "
                 << list.last().toDouble();
        if (once)
        {
            mDateTimeEnd = dateTime;
            once         = false;
        }

        double val = list.last().toDouble();

        if (val > mMax)
            mMax = val;
        if (val < mMin)
            mMin = val;

        if (dateTime.date() < dateTime.currentDateTime().date())
            mSeries->first.append(dateTime.toMSecsSinceEpoch(), val);
        else if (dateTime.date() == dateTime.currentDateTime().date())
        {
            mSeries->first.append(dateTime.toMSecsSinceEpoch(), val);
            mSeries->second.append(dateTime.toMSecsSinceEpoch(), val);
        }
        else
            mSeries->second.append(dateTime.toMSecsSinceEpoch(), val);
    }

    file.close();
    mDateTimeStart = dateTime;
}

QString FuelPriceChart::getChartDataFileName(Common::FuelType fuelType)
{

#ifdef __linux__
    // miki
    const QString pathFuelPrices = "../stacja-paliwowa/FuelPricesChartData/";
#elif _WIN32
    const QString pathFuelPrices =
        "E:\\Studia\\9_sem\\Projekty\\System_zarzadzania_stacja_paliw\\stacja-"
        "paliwowa\\FuelPricesChartData\\";
//    const QString pathFuelPrices = "C:\\Users\\Igor\\Documents\\stacja\\stacja_master\\stacja-"
//                                   "paliwowa\\FuelPricesChartData\\";
#else
#error Not supported
#endif

    switch (fuelType)
    {
    case Common::FuelType::ON:
        return pathFuelPrices + QString("ON.csv");
    case Common::FuelType::OO:
        return pathFuelPrices + QString("OO.csv");
    case Common::FuelType::PB95:
        return pathFuelPrices + QString("PB95.csv");
    case Common::FuelType::PB98:
        return pathFuelPrices + QString("PB98.csv");
    default:
        return QString();
    }
}

void FuelPriceChart::displayDebugChart()
{
    QSplineSeries* series  = new QSplineSeries();
    QSplineSeries* series2 = new QSplineSeries();

    series2->setPen(QPen(QColor(Qt::GlobalColor::red)));

    QRandomGenerator rand;
    for (int i = 1; i < 31; i++)
    {
        QDateTime date;
        date.setDate(QDate(2018, 11, i));
        series->append(date.toMSecsSinceEpoch(), qreal(rand.generateDouble() + 4));
    }
    for (int i = 1; i < 15; i++)
    {
        QDateTime date;
        date.setDate(QDate(2018, 12, i));
        series->append(date.toMSecsSinceEpoch(), qreal(rand.generateDouble() + 4));
    }
    QDateTime date;
    date.setDate(QDate(2018, 12, 15));
    qreal tmpVal(rand.generateDouble() + 4);
    series->append(date.toMSecsSinceEpoch(), tmpVal);
    series2->append(date.toMSecsSinceEpoch(), tmpVal);
    for (int i = 16; i < 32; i++)
    {
        QDateTime date;
        date.setDate(QDate(2018, 12, i));
        series2->append(date.toMSecsSinceEpoch(), qreal(rand.generateDouble() + 4));
    }

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->addSeries(series2);
    chart->legend()->hide();
    chart->setTitle("testowo");

    QDateTimeAxis* axisX = new QDateTimeAxis;
    axisX->setTickCount(31);
    axisX->setFormat("dd MMM yyyy");
    axisX->setLabelsAngle(90);
    axisX->setTitleText("Date");
    axisX->setRange(QDateTime(QDate(2018, 11, 1)), QDateTime(QDate(2018, 12, 31)));
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    series2->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis;
    axisY->setLabelFormat("%.2f");
    axisY->setTickCount(15);
    axisY->setTitleText("Sunspots count");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    series2->attachAxis(axisY);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(mUi->FuelPriceTabChartLayout->geometry().width(),
                      mUi->FuelPriceTabChartLayout->geometry().height());

    // create layout
    QGridLayout layout;
    layout.addWidget(chartView);
    mUi->FuelPriceTabChartLayout->addLayout(&layout);
}

} // namespace BackEnd
