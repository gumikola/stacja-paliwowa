#ifndef UPDATETANKSFILLLEVEL_H
#define UPDATETANKSFILLLEVEL_H

#include "DataBase/DataBaseApi.h"
#include "QLineEdit"
#include "ui_mainwindow.h"
#include <QComboBox>
#include <QObject>
#include <QPushButton>

namespace BackEnd {

class UpdateTanksFillLevel : public QObject
{
    Q_OBJECT
    DataBaseApi::DataBaseApi& mDataBaseApi;
    QComboBox&                mOperationType;
    QComboBox&                mTankType;
    QComboBox&                mChosenTank;
    QPushButton&              mUpdateButton;
    QLineEdit&                mAmount;

public:
    UpdateTanksFillLevel(Ui::MainWindow* ui, DataBaseApi::DataBaseApi& dataBaseApi);

private slots:
    void operationTypeChanged(int);
    void tankTypeChanged(int);
    void chosenTankChanged(int);
    void amountChanged();
    void updateButtonPressed(void);

private:
    void     setDefault(uint step);
    uint     getAmount(void);
    uint32_t calculateTankFillLevel();

signals:
    void tanksFillLevelChanged(void);
};

} // namespace BackEnd

#endif // UPDATETANKSFILLLEVEL_H
