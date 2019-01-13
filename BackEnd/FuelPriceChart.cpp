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
    , mChart(new QChart)
    , mAxisX(new QDateTimeAxis)
    , mAxisY(new QValueAxis)
    , mChartView(new QChartView(mChart))
{
    fillFuelPricesPaths();
    fillFuelTypeComboBox();

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
    mAxisX->setTickCount(30);
    mAxisX->setRange(mDateTimeStart, mDateTimeEnd);
    mChart->addAxis(mAxisX, Qt::AlignBottom);

    for (ChartSeries series : mSerieses)
    {
        series.past->attachAxis(mAxisX);
        series.predicted->attachAxis(mAxisX);
    }
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

    for (ChartSeries series : mSerieses)
    {
        series.past->attachAxis(mAxisY);
        series.predicted->attachAxis(mAxisY);
    }
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

    for (ChartSeries series : mSerieses)
    {
        mChart->addSeries(series.past.get());
        mChart->addSeries(series.predicted.get());
    }

    mChart->setAnimationOptions(QChart::SeriesAnimations);
    mChart->setTitle(QString("Wykres cen paliwa typu: ") + mChosenFuelType.currentText());
}

void FuelPriceChart::printChart()
{
    qDebug() << __PRETTY_FUNCTION__;

    if (mChosenFuelType.currentIndex() <= 0)
        throw QString("FuelPriceChart::printChart Wrong fuel type");

    int index = mChosenFuelType.currentIndex();

    fillSerieses(static_cast<Common::FuelType>(index - 1));

    uint colorId = static_cast<uint>(Qt::GlobalColor::red);

    QPen pen;
    pen.setWidth(2);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    for (ChartSeries series : mSerieses)
    {
        pen.setBrush(static_cast<Qt::GlobalColor>(colorId++));

        pen.setStyle(Qt::SolidLine);
        pen.setWidth(2);
        series.past->setPen(pen);

        pen.setWidth(2);
        pen.setStyle(Qt::DotLine);
        series.predicted->setPen(pen);
    }
    setUpChartView();
    setUpChart();
    setUpXAxis();
    setUpYAxis();
}

void FuelPriceChart::fillFuelPricesPaths()
{
    QStringList producersNames = getProducersNames();

    qDebug() << __PRETTY_FUNCTION__ << " Found producers:" << producersNames;

    QPair<Common::FuelType, QString> paths;
    for (QString producerName : producersNames)
    {
        QString pathToProducer = getChartsDirectory() + producerName + QString("/");
        QDir    chartsDir(pathToProducer);

        const QStringList& files = chartsDir.entryList(QDir::Files);

        for (QString fileName : files)
        {
            QString pathToPrices(pathToProducer + fileName);

            if (mPathsToFuelPrices.contains(getFuelTypeFromFileName(fileName)))
                mPathsToFuelPrices[getFuelTypeFromFileName(fileName)].push_back(pathToPrices);
            else
                mPathsToFuelPrices.insert(getFuelTypeFromFileName(fileName),
                                          QStringList(pathToPrices));
        }
    }
}

const QString FuelPriceChart::getChartsDirectory()
{
    QString absolutePathToChartsPricesData =
        QDir::currentPath().split("build")[0] + mChartsPricesDataPath;

    qDebug() << __PRETTY_FUNCTION__ << " Charts data directory: " << absolutePathToChartsPricesData;

    return absolutePathToChartsPricesData;
}

void FuelPriceChart::fillFuelTypeComboBox()
{
    qDebug() << __PRETTY_FUNCTION__;

    QList<Common::FuelType> fuelTypes = mPathsToFuelPrices.keys();

    mChosenFuelType.addItem("Wybierz typ paliwa.");

    for (Common::FuelType fuelType : fuelTypes)
        mChosenFuelType.addItem(Common::getFuelTypeName(fuelType));
}

const QStringList FuelPriceChart::getProducersNames()
{
    qDebug() << __PRETTY_FUNCTION__;
    QDir chartsDir(getChartsDirectory());

    return chartsDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
}

const QMap<QDateTime, double> FuelPriceChart::getPricesFromFile(const QString filePath)
{
    qDebug() << __PRETTY_FUNCTION__;

    QMap<QDateTime, double> prices;

    QFile file(filePath);

    if (not file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw QString(__PRETTY_FUNCTION__) + QString(" File path: ") + filePath;

    QTextStream fileStream(&file);

    while (!fileStream.atEnd())
    {
        QStringList list = fileStream.readLine().split(",");
        QDateTime   dateTime;
        dateTime.setDate(QDate::fromString(list.first(), "dd-MM-yyyy"));
        prices.insert(dateTime, list.last().toDouble());
    }

    file.close();

    return prices;
}

void FuelPriceChart::updateYMax(const QMap<QDateTime, double> data)
{
    qDebug() << __PRETTY_FUNCTION__;

    auto max = std::max_element(data.begin(), data.end(),
                                [](const double& p1, const double& p2) { return p1 < p2; });

    if (max.value() > mMax)
        mMax = max.value();
}

void FuelPriceChart::updateYMin(const QMap<QDateTime, double> data)
{
    qDebug() << __PRETTY_FUNCTION__;

    auto min = std::min_element(data.begin(), data.end(),
                                [](const double& p1, const double& p2) { return p1 < p2; });

    if (min.value() < mMin)
        mMin = min.value();
}

Common::FuelType FuelPriceChart::getFuelTypeFromFileName(const QString& filename)
{
    qDebug() << __PRETTY_FUNCTION__;

    QStringList tmp = filename.split(".");

    return Common::getFuelTypeEnum(tmp.first());
}

ChartSeries FuelPriceChart::fillSeries(const QMap<QDateTime, double>& fuelPrices)
{
    qDebug() << __PRETTY_FUNCTION__;

    ChartSeries series;
    QDateTime   dateTime;
    QDate       currentDate = dateTime.currentDateTime().date();

    for (auto it = fuelPrices.begin(); it != fuelPrices.end(); it++)
    {
        if (it.key().date() < currentDate)
        {
            series.past->append(it.key().toMSecsSinceEpoch(), it.value());
            ;
        }
        else
        {
            if (series.past->count() == 0)
                series.past->append(it.key().toMSecsSinceEpoch(), it.value());

            series.predicted->append(it.key().toMSecsSinceEpoch(), it.value());
        }
    }

    if (mDateTimeStart > fuelPrices.firstKey())
        mDateTimeStart = fuelPrices.firstKey();

    if (mDateTimeEnd < fuelPrices.lastKey())
        mDateTimeEnd = fuelPrices.lastKey();

    updateYMax(fuelPrices);
    updateYMin(fuelPrices);

    return series;
}

QString FuelPriceChart::getProducerNameFromPath(const QString& path)
{
    QString tmpPath = path;
    tmpPath.remove(getChartsDirectory());
    QStringList tmp = tmpPath.split("/");

    return tmp.first();
}

void FuelPriceChart::fillSerieses(Common::FuelType fuelType)
{
    qDebug() << __PRETTY_FUNCTION__;

    mSerieses.clear();

    mSize          = 0;
    mMin           = 60000;
    mMax           = 0;
    mDateTimeStart = mDateTimeStart.currentDateTime(); // always higher then dates in prices file
    mDateTimeEnd.setTime_t(0);                         // linux time start data (1970)

    for (QString path : mPathsToFuelPrices[fuelType])
    {
        ChartSeries series = fillSeries(getPricesFromFile(path));
        series.past->setName(getProducerNameFromPath(path));
        mSerieses.push_back(series);
    }
}

ChartSeries::ChartSeries()
{
    qDebug() << __PRETTY_FUNCTION__;
    past.reset(new QSplineSeries);
    predicted.reset(new QSplineSeries);
}

ChartSeries::~ChartSeries()
{
    qDebug() << __PRETTY_FUNCTION__;
}

} // namespace BackEnd
