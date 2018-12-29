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

UpdateTanksFillLevel::UpdateTanksFillLevel(Ui::MainWindow* ui, DataBaseApi::DataBaseApi& dataBaseApi)
    : mDataBaseApi(dataBaseApi)
    , mOperationType(*ui->changeTankFillLevelTabOperatonType)
    , mTankType(*ui->changeTankFillLevelTabTankType)
    , mChosenTank(*ui->changeTankFillLevelTabChosenTank)
    , mUpdateButton(*ui->changeTankFillLevelTabUpdateButton)
    , mAmount(*ui->changeTankFillLevelTabAmount)
{
    setDefault(0);
    connect(ui->changeTankFillLevelTabOperatonType, SIGNAL(activated(int)), this, SLOT(operationTypeChanged(int)));
    connect(ui->changeTankFillLevelTabTankType, SIGNAL(activated(int)), this, SLOT(tankTypeChanged(int)));
    connect(ui->changeTankFillLevelTabChosenTank, SIGNAL(activated(int)), this, SLOT(chosenTankChanged(int)));
    connect(ui->changeTankFillLevelTabAmount, SIGNAL(editingFinished()), this, SLOT(amountChanged()));
    connect(ui->changeTankFillLevelTabUpdateButton, SIGNAL(pressed()), this, SLOT(updateButtonPressed()));
}

uint UpdateTanksFillLevel::getAmount(void)
{
    QString tmp = mAmount.text();

    if (not tmp.size())
        throw QString("Pole \"Ilość\" nie moze byc puste!");

    bool result = true;
    uint amount = tmp.toUInt(&result);

    if (result == false)
        throw QString("Niepoprawna \"Ilość\" paliwa! (w tym polu dopuszczalne są tylk oliczby");

    return amount;
}

void UpdateTanksFillLevel::setDefault(uint step)
{
    switch (step)
    {
    case 0:
        mOperationType.setEnabled(true);
        mOperationType.setCurrentIndex(0);
        [[clang::fallthrough]];
    case 1:
        mTankType.setEnabled(false);
        mTankType.setCurrentIndex(0);
        [[clang::fallthrough]];
    case 2:
        mChosenTank.setEnabled(false);
        mChosenTank.setCurrentIndex(0);
        mChosenTank.addItem(QString("wybierz zbiornik"));
        [[clang::fallthrough]];
    case 3:
        mAmount.setText(QString(""));
        mAmount.setEnabled(false);
        [[clang::fallthrough]];
    case 4:
        mUpdateButton.setEnabled(false);
        break;
    }
}

void UpdateTanksFillLevel::operationTypeChanged(int index)
{
    qDebug("operationTypeChanged to: %d", index);
    if (index)
    {
        mTankType.setEnabled(true);
    }
    else
    {
        setDefault(1);
    }
}

void UpdateTanksFillLevel::tankTypeChanged(int index)
{
    qDebug("tankTypeChanged to: %d", index);

    if (index)
    {
        mChosenTank.setEnabled(true);

        mChosenTank.clear();
        mChosenTank.addItem(QString("wybierz zbiornik"));

        if (index == 1)
        {
            mChosenTank.addItem(QString("ON 1"));
            mChosenTank.addItem(QString("ON 2"));
            mChosenTank.addItem(QString("ON 3"));
            mChosenTank.addItem(QString("OO"));
        }
        else if (index == 2)
        {
            mChosenTank.addItem(QString("ON"));
            mChosenTank.addItem(QString("PB95"));
            mChosenTank.addItem(QString("PB98"));
        }
    }
    else
    {
        setDefault(2);
    }
}

void UpdateTanksFillLevel::chosenTankChanged(int index)
{
    qDebug("chosenTankChanged to: %d", index);

    if (index)
    {
        mAmount.setEnabled(true);
    }
    else
    {
        setDefault(3);
    }
}

void UpdateTanksFillLevel::amountChanged()
{
    try
    {
        qDebug("amountChanged to: %d l", getAmount());
        mUpdateButton.setEnabled(true);
    }
    catch (QString& e)
    {
        setDefault(4);
        QMessageBox msgBox;
        msgBox.setFont(QFont(QString("Arial"), 14));
        msgBox.setText(e);
        msgBox.exec();
    }
}

uint32_t UpdateTanksFillLevel::calculateTankFillLevel()
{
    if (mOperationType.currentIndex() == 1)
        return getAmount();

    qDebug("Debug value = %u", static_cast<uint>(Common::getFuelTankEnum(mChosenTank.currentText())));

    uint32_t actualFillLvl = mDataBaseApi.getTanksFillLevel()[Common::getFuelTankEnum(mChosenTank.currentText())];

    if (mOperationType.currentIndex() == 2)
        return actualFillLvl + getAmount();
    else if (mOperationType.currentIndex() == 3)
        return actualFillLvl - getAmount();
    else
    {
        qDebug("UpdateTanksFillLevel::calculateTankFillLevel() wrong operation type!");
        throw QString("UpdateTanksFillLevel::calculateTankFillLevel() wrong operation type!");
    }
}

void UpdateTanksFillLevel::updateButtonPressed()
{
    try
    {
        qDebug("updateButtonPressed");
        QMap<Common::FuelTankType, uint32_t> tmp = mDataBaseApi.getTanksFillLevel();
        mDataBaseApi.updateTankFillLevel(Common::getFuelTankEnum(mChosenTank.currentText()), calculateTankFillLevel());
        emit tanksFillLevelChanged();

        QMessageBox msgBox;
        msgBox.setFont(QFont(QString("Arial"), 14));
        msgBox.setText(QString("Pomyślnie zapisano do bazy danych."));
        msgBox.exec();
    }
    catch (QString& e)
    {
        qDebug(QString(QString("Exeption cought: ") + e).toStdString().c_str());
    }
}

} // namespace BackEnd
