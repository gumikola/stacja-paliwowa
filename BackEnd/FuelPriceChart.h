#ifndef FUELPRICECHART_H
#define FUELPRICECHART_H

#include <Common.h>
#include "DataBase/DataBaseApi.h"
#include "ui_mainwindow.h"
#include <QComboBox>
#include <QObject>
#include <QPair>
#include <QSharedPointer>
#include <QSplineSeries>
#include <QString>
#include <QtCharts>

namespace BackEnd {

struct ChartSeries
{
    QSharedPointer<QSplineSeries> past;
    QSharedPointer<QSplineSeries> predicted;

    ChartSeries();

    ~ChartSeries();
};

class FuelPriceChart : public QObject
{
    Q_OBJECT

    DataBaseApi::DataBaseApi mDatabaseApi;
    Ui::MainWindow*          mUi;
    QComboBox&               mChosenFuelType;

    QVector<ChartSeries>                mSeries;
    QChart*                             mChart;
    QDateTimeAxis*                      mAxisX;
    QValueAxis*                         mAxisY;
    QChartView*                         mChartView;
    QDateTime                           mDateTimeEnd;
    QDateTime                           mDateTimeStart;
    uint                                mSize;
    double                              mMax;
    double                              mMin;
    QMap<Common::FuelType, QStringList> mPathsToFuelPrices;

    void                          fillSeries(Common::FuelType fuelType);
    void                          setUpXAxis();
    void                          setUpYAxis();
    void                          setUpChartView();
    void                          setUpChart();
    void                          printChart();
    void                          fillFuelPricesPaths();
    const QString                 getChartsDirectory();
    void                          fillFuelTypeComboBox();
    const QStringList             getProducersNames();
    const QMap<QDateTime, double> getPricesFromFile(const QString filePath);
    void                          updateYMax(const QMap<QDateTime, double> data);
    void                          updateYMin(const QMap<QDateTime, double> data);
    Common::FuelType              getFuelTypeFromFileName(const QString& filename);
    ChartSeries                   fillSeries(const QMap<QDateTime, double>& fuelPrices);
    QString                       getProducerNameFromPath(const QString& path);

public:
    FuelPriceChart(Ui::MainWindow* ui, DataBaseApi::DataBaseApi& databaseApi);

signals:

public slots:
    void chosenFuelTypeChanged(int index);
    void tabChanged(int id);
};

} // namespace BackEnd

#endif // FUELPRICECHART_H
