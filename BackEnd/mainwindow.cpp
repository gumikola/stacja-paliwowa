#include "mainwindow.h"
#include "AddOrder.h"
#include "Orders.h"
#include "ui_mainwindow.h"
#include <QMap>
#include <QScopedPointer>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeObjects()
{
    // first set fill level of fuel tanks
    QMap<Common::FuelTankType, uint32_t> fuelTanksFillLevel = mDataBaseApi.getTanksFillLevel();

    ui->FuelTankStation95->display(static_cast<int>(fuelTanksFillLevel[Common::FuelTankType::PB95]));
    ui->FuelTankStation98->display(static_cast<int>(fuelTanksFillLevel[Common::FuelTankType::PB98]));
    ui->FuelTankStationON->display(static_cast<int>(fuelTanksFillLevel[Common::FuelTankType::ON]));
    ui->FuelTankStorageON1->display(static_cast<int>(fuelTanksFillLevel[Common::FuelTankType::ON1]));
    ui->FuelTankStorageON2->display(static_cast<int>(fuelTanksFillLevel[Common::FuelTankType::ON2]));
    ui->FuelTankStorageON3->display(static_cast<int>(fuelTanksFillLevel[Common::FuelTankType::ON3]));
    ui->FuelTankStorageOO4->display(static_cast<int>(fuelTanksFillLevel[Common::FuelTankType::OO]));

    // #TO_DO

    // making objects
    mOrders.reset(new BackEnd::Orders(ui, mDataBaseApi));
    mAddOrder.reset(new BackEnd::AddOrder(ui, mDataBaseApi));
}
