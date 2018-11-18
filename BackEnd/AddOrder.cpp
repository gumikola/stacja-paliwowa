#include "AddOrder.h"
#include <QComboBox>
#include <QLineEdit>
#include <QMainWindow>
#include <QObject>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include "DataBase/DataBaseApi.h"
#include "ui_mainwindow.h"

namespace BackEnd {

AddOrder::AddOrder(Ui::MainWindow *ui, DataBaseApi::DataBaseApi &databaseApi)
    : mAddOrderButton(*ui->addOrderTabAddOrderButton),
      mCalculateButton(*ui->addOrderTabCalculateButton),
      mAmount(*ui->addOrderTabAmount),
      mCity(*ui->addOrderTabCity),
      mIncome(*ui->addOrderTabIncome),
      mMargin(*ui->addOrderTabMargin),
      mNumber(*ui->addOrderTabTotalPrice),
      mOrdererName(*ui->addOrderTabOrdererName),
      mStreet(*ui->addOrderTabStreet),
      mTotalPrice(*ui->addOrderTabTotalPrice),
      mFuelTypeBox(*ui->addOrderTabFuelTypeBox),
      mDatabaseApi(databaseApi) {
  connect(ui->addOrderTabAddOrderButton, SIGNAL(pressed()), this,
          SLOT(addOrderPressed()));
  connect(ui->addOrderTabCalculateButton, SIGNAL(pressed()), this,
          SLOT(calculatePressed()));
}

void AddOrder::calculatePressed() { qDebug("calculatePressed"); }

void AddOrder::addOrderPressed() { qDebug("addOrderPressed"); }
}  // namespace BackEnd
