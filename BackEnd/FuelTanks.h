#pragma once

#include "Common.h"
#include "ui_mainwindow.h"
#include <QObject>

namespace DataBaseApi {
class DataBaseApi;
} // namespace DataBaseApi

namespace BackEnd {

class FuelTanks : public QObject
{
    Q_OBJECT
    Ui::MainWindow*           mUi;
    DataBaseApi::DataBaseApi& mDatabaseApi;

public:
    explicit FuelTanks(Ui::MainWindow* ui, DataBaseApi::DataBaseApi& databaseApi);
    void displayEditWindow(Common::FuelTankType tank);

signals:

public slots:
    void editONPressed();
    void editON1Pressed();
    void editON2Pressed();
    void editON3Pressed();
    void editOOPressed();
    void editPB95Pressed();
    void editPB98Pressed();
    void displayTanksFillLevel(void);
};

} // namespace BackEnd
