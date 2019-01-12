#include "FuelTanks.h"
#include "DataBase/DataBaseApi.h"
#include "UpdateTanksFillLevel.h"

namespace BackEnd {

FuelTanks::FuelTanks(Ui::MainWindow* ui, DataBaseApi::DataBaseApi& databaseApi)
    : mUi(ui)
    , mDatabaseApi(databaseApi)
{
    displayTanksFillLevel();
    connect(ui->ONButton, SIGNAL(pressed()), this, SLOT(editONPressed()));
    connect(ui->ON1Button, SIGNAL(pressed()), this, SLOT(editON1Pressed()));
    connect(ui->ON2Button, SIGNAL(pressed()), this, SLOT(editON2Pressed()));
    connect(ui->ON3Button, SIGNAL(pressed()), this, SLOT(editON3Pressed()));
    connect(ui->OOButton, SIGNAL(pressed()), this, SLOT(editOOPressed()));
    connect(ui->PB95Button, SIGNAL(pressed()), this, SLOT(editPB95Pressed()));
    connect(ui->PB98Button, SIGNAL(pressed()), this, SLOT(editPB98Pressed()));
    connect(ui->OptionsTab, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
}

void FuelTanks::displayEditWindow(Common::FuelTankType tank)
{
    qDebug() << __PRETTY_FUNCTION__;

    UpdateTanksFillLevel window(mDatabaseApi, tank);

    QObject::connect(&window, SIGNAL(tanksFillLevelChanged()), this, SLOT(displayTanksFillLevel()));

    window.exec();
}

void FuelTanks::displayTanksFillLevel()
{
    qDebug() << __PRETTY_FUNCTION__;

    QMap<Common::FuelTankType, uint32_t> fuelTanksFillLevel = mDatabaseApi.getTanksFillLevel();

    mUi->FuelTankStation95->display(
        static_cast<int>(fuelTanksFillLevel[Common::FuelTankType::PB95]));
    mUi->FuelTankStation98->display(
        static_cast<int>(fuelTanksFillLevel[Common::FuelTankType::PB98]));
    mUi->FuelTankStationON->display(static_cast<int>(fuelTanksFillLevel[Common::FuelTankType::ON]));
    mUi->FuelTankStorageON1->display(
        static_cast<int>(fuelTanksFillLevel[Common::FuelTankType::ON1]));
    mUi->FuelTankStorageON2->display(
        static_cast<int>(fuelTanksFillLevel[Common::FuelTankType::ON2]));
    mUi->FuelTankStorageON3->display(
        static_cast<int>(fuelTanksFillLevel[Common::FuelTankType::ON3]));
    mUi->FuelTankStorageOO4->display(
        static_cast<int>(fuelTanksFillLevel[Common::FuelTankType::OO]));
}

void FuelTanks::tabChanged(int id)
{
    qDebug() << __PRETTY_FUNCTION__;

    if (id == 1)
    {
        displayTanksFillLevel();
    }
}

void FuelTanks::editONPressed()
{
    qDebug() << __PRETTY_FUNCTION__;

    displayEditWindow(Common::FuelTankType::ON);
}

void FuelTanks::editON1Pressed()
{
    qDebug() << __PRETTY_FUNCTION__;

    displayEditWindow(Common::FuelTankType::ON1);
}

void FuelTanks::editON2Pressed()
{
    qDebug() << __PRETTY_FUNCTION__;

    displayEditWindow(Common::FuelTankType::ON2);
}

void FuelTanks::editON3Pressed()
{
    qDebug() << __PRETTY_FUNCTION__;

    displayEditWindow(Common::FuelTankType::ON3);
}

void FuelTanks::editOOPressed()
{
    qDebug() << __PRETTY_FUNCTION__;

    displayEditWindow(Common::FuelTankType::OO);
}

void FuelTanks::editPB95Pressed()
{
    qDebug() << __PRETTY_FUNCTION__;

    displayEditWindow(Common::FuelTankType::PB95);
}

void FuelTanks::editPB98Pressed()
{
    qDebug() << __PRETTY_FUNCTION__;

    displayEditWindow(Common::FuelTankType::PB98);
}

} // namespace BackEnd
