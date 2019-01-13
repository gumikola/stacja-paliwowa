#ifndef FUELPRICECHART_H
#define FUELPRICECHART_H

#include <Common.h>
#include "Common.h"
#include "DataBase/DataBaseApi.h"
#include "ui_mainwindow.h"
#include <QComboBox>
#include <QObject>
#include <QPair>
#include <QSplineSeries>
#include <QString>
#include <QtCharts>

namespace BackEnd {

class FuelPriceChart : public QObject
{
    Q_OBJECT

    DataBaseApi::DataBaseApi mDatabaseApi;
    Ui::MainWindow*          mUi;
    QComboBox&               mChosenFuelType;

    QPair<QSplineSeries, QSplineSeries>* mSeries;
    QChart*                              mChart;
    QDateTimeAxis*                       mAxisX;
    QValueAxis*                          mAxisY;
    QChartView*                          mChartView;
    QDateTime                            mDateTimeEnd;
    QDateTime                            mDateTimeStart;
    uint                                 mSize;
    double                               mMax;
    double                               mMin;

    void    fillSeries(Common::FuelType fuelType);
    QString getChartDataFileName(Common::FuelType);
    void    setUpXAxis();
    void    setUpYAxis();
    void    setUpChartView();
    void    setUpChart();
    void    printChart();

public:
    FuelPriceChart(Ui::MainWindow* ui, DataBaseApi::DataBaseApi& databaseApi);

signals:

public slots:
    void chosenFuelTypeChanged(int index);
    void tabChanged(int id);
};

} // namespace BackEnd

#endif // FUELPRICECHART_H
