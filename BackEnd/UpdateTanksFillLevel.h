#ifndef UPDATETANKSFILLLEVEL_H
#define UPDATETANKSFILLLEVEL_H

#include <Common.h>
#include "ui_edit_tank_fill_lvl.h"
#include <QDialog>
#include <QMessageBox>
#include <QObject>

namespace DataBaseApi {
class DataBaseApi;
} // namespace DataBaseApi

namespace BackEnd {

class UpdateTanksFillLevel : public QObject
{
    Q_OBJECT
    DataBaseApi::DataBaseApi&   mDataBaseApi;
    Ui_EditTankFillLevel*       mUi;
    const Common::FuelTankType& mTank;
    QDialog                     mDialog;

public:
    UpdateTanksFillLevel(DataBaseApi::DataBaseApi& dataBaseApi, const Common::FuelTankType& tank);
    void exec();

private slots:
    void operationTypeChanged(int);
    void amountChanged();
    void updateButtonPressed(void);

private:
    uint     getAmount(void);
    uint32_t calculateTankFillLevel();

signals:
    void tanksFillLevelChanged(void);
};

} // namespace BackEnd

#endif // UPDATETANKSFILLLEVEL_H
