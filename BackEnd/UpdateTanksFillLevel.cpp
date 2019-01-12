#include "UpdateTanksFillLevel.h"
#include "Common.h"
#include "DataBase/DataBaseApi.h"
#include "QLineEdit"
#include "mainwindow.h"
#include <QComboBox>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QString>

namespace BackEnd {

UpdateTanksFillLevel::UpdateTanksFillLevel(DataBaseApi::DataBaseApi&   dataBaseApi,
                                           const Common::FuelTankType& tank)
    : mDataBaseApi(dataBaseApi)
    , mUi(new Ui::EditTankFillLevel)
    , mTank(tank)
{
    mUi->setupUi(&mDialog);
    connect(mUi->operatonType, SIGNAL(activated(int)), this, SLOT(operationTypeChanged(int)));
    connect(mUi->amount, SIGNAL(editingFinished()), this, SLOT(amountChanged()));
    connect(mUi->updateButton, SIGNAL(pressed()), this, SLOT(updateButtonPressed()));

    mDialog.open();
}

void UpdateTanksFillLevel::exec()
{
    qDebug() << __PRETTY_FUNCTION__;

    mDialog.exec();
}

uint UpdateTanksFillLevel::getAmount(void)
{
    qDebug() << __PRETTY_FUNCTION__;

    QString tmp = mUi->amount->text();

    if (not tmp.size())
        throw QString("Pole \"Ilość\" nie moze byc puste!");

    bool result = true;
    uint amount = tmp.toUInt(&result);

    if (result == false)
        throw QString("Niepoprawna \"Ilość\" paliwa! (w tym polu dopuszczalne są tylk oliczby");

    return amount;
}

void UpdateTanksFillLevel::operationTypeChanged(int index)
{
    qDebug() << __PRETTY_FUNCTION__ << " Operation type changed to: " << index;

    if (index)
    {
        mUi->amount->setEnabled(true);
        mUi->updateButton->setEnabled(true);
    }
    else
    {
        mUi->amount->setEnabled(false);
        mUi->updateButton->setEnabled(false);
    }
}

void UpdateTanksFillLevel::amountChanged()
{
    qDebug() << __PRETTY_FUNCTION__;

    mUi->updateButton->setEnabled(true);
}

uint32_t UpdateTanksFillLevel::calculateTankFillLevel()
{
    qDebug() << __PRETTY_FUNCTION__;

    if (mUi->operatonType->currentIndex() == 1)
        return getAmount();

    uint32_t actualFillLvl = mDataBaseApi.getTanksFillLevel()[mTank];

    if (mUi->operatonType->currentIndex() == 2)
        return actualFillLvl + getAmount();
    else if (mUi->operatonType->currentIndex() == 3)
        return actualFillLvl - getAmount();
    else
    {
        qDebug() << __PRETTY_FUNCTION__ << " Wrong operation type!";

        throw QString("UpdateTanksFillLevel::calculateTankFillLevel() wrong operation type!");
    }
}

void UpdateTanksFillLevel::updateButtonPressed()
{
    qDebug() << __PRETTY_FUNCTION__;
    try
    {
        qDebug("updateButtonPressed");
        QMap<Common::FuelTankType, uint32_t> tmp = mDataBaseApi.getTanksFillLevel();
        mDataBaseApi.updateTankFillLevel(mTank, calculateTankFillLevel());
        emit tanksFillLevelChanged();

        QMessageBox msgBox;
        msgBox.setFont(QFont(QString("Arial"), 14));
        msgBox.setText(QString("Pomyślnie zapisano do bazy danych."));
        msgBox.exec();

        mDialog.close();
    }
    catch (QString& e)
    {
        qDebug() << (QString("Exeption cought: ") + e);
    }
}

} // namespace BackEnd
