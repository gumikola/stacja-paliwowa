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
    QComboBox&               mFuelPriceChartChoosedFuelType;

    QPair<QSplineSeries, QSplineSeries>* mSeries;
    QChart*                              mChart;
    QDateTimeAxis*                       mAxisX;
    QValueAxis*                          mAxisY;
    QChartView*                          mChartView;
    QDateTime                            mDateTimeEnd;
    QDateTime                            mDateTimeStart;
    uint                                 mSize;

    void    fillSeries(Common::FuelType fuelType);
    QString getChartDataFileName(Common::FuelType);
    void    displayDebugChart(void);
    void    setUpXAxis();
    void    setUpYAxis();
    void    setUpChartView();
    void    setUpChart();
    void    removeAndCreateNew();

public:
    FuelPriceChart(Ui::MainWindow* ui, DataBaseApi::DataBaseApi& databaseApi);

signals:

public slots:
    void choosedFuelTypeChanged(int index);
};

} // namespace BackEnd

#endif // FUELPRICECHART_H
