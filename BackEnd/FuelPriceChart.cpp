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
    , mChosenFuelType(*ui->FuelPriceTabFuelType)
    , mSeries(new QPair<QSplineSeries, QSplineSeries>)
    , mChart(new QChart)
    , mAxisX(new QDateTimeAxis)
    , mAxisY(new QValueAxis)
    , mChartView(new QChartView(mChart))
{
    connect(ui->FuelPriceTabFuelType, SIGNAL(activated(int)), this,
            SLOT(chosenFuelTypeChanged(int)));
    connect(ui->OptionsTab, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    setUpChartView();
    mUi->FuelPriceTabChartLayout->addWidget(mChartView);
}

void FuelPriceChart::chosenFuelTypeChanged(int index)
{
    qDebug() << __PRETTY_FUNCTION__ << " index=" << index;

    if (mChosenFuelType.currentIndex() <= 0)
        return;
    printChart();
}

void FuelPriceChart::tabChanged(int id)
{
    if (id == 2)
    {
        qDebug() << __PRETTY_FUNCTION__;

        if (mChosenFuelType.currentIndex() > 0)
            printChart();
    }
}

void FuelPriceChart::setUpXAxis()
{
    qDebug() << __PRETTY_FUNCTION__;

    delete mAxisX;
    mAxisX = new QDateTimeAxis;

    mAxisX->setFormat("dd MMM yyyy");
    mAxisX->setLabelsAngle(90);
    mAxisX->setTitleText("Data (dd mm rrrr)");
    mAxisX->setTickCount(mSize / 2);
    mAxisX->setRange(mDateTimeStart, mDateTimeEnd);
    mChart->addAxis(mAxisX, Qt::AlignBottom);
    mSeries->first.attachAxis(mAxisX);
    mSeries->second.attachAxis(mAxisX);
}

void FuelPriceChart::setUpYAxis()
{
    qDebug() << __PRETTY_FUNCTION__;

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
    qDebug() << __PRETTY_FUNCTION__;

    mUi->FuelPriceTabChartLayout->removeWidget(mChartView);
    delete mChartView;

    mChart     = new QChart;
    mChartView = new QChartView(mChart);

    mUi->FuelPriceTabChartLayout->addWidget(mChartView);
    mChartView->setRenderHint(QPainter::Antialiasing);
    mChartView->resize(mUi->FuelPriceTabChartLayout->geometry().width(),
                       mUi->FuelPriceTabChartLayout->geometry().height());
}

void FuelPriceChart::setUpChart()
{
    qDebug() << __PRETTY_FUNCTION__;

    mChart->addSeries(&mSeries->first);
    mChart->addSeries(&mSeries->second);
    mChart->legend()->hide();
    mChart->setTitle(QString("Wykres cen paliwa typu: ") + mChosenFuelType.currentText());
}

void FuelPriceChart::printChart()
{
    qDebug() << __PRETTY_FUNCTION__;

    if (mChosenFuelType.currentIndex() <= 0)
        throw QString("FuelPriceChart::printChart Wrong fuel type");

    int index = mChosenFuelType.currentIndex();

    delete mSeries;
    mSeries = new QPair<QSplineSeries, QSplineSeries>;

    fillSeries(static_cast<Common::FuelType>(index - 1));

    mSeries->first.setPen(QPen(QColor(Qt::GlobalColor::blue)));
    mSeries->second.setPen(QPen(QColor(Qt::GlobalColor::red)));

    setUpChartView();
    setUpChart();
    setUpXAxis();
    setUpYAxis();
}

void FuelPriceChart::fillSeries(Common::FuelType fuelType)
{
    qDebug() << __PRETTY_FUNCTION__;

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
    qDebug() << __PRETTY_FUNCTION__;

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

} // namespace BackEnd
