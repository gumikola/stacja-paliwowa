#include "AddOrder.h"
#include <QComboBox>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QPair>
#include <QPushButton>
#include <QString>
#include <QTableWidget>
#include <QTextEdit>

#include "Common.h"
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
      mNumber(*ui->addOrderTabNumber),
      mOrdererName(*ui->addOrderTabOrdererName),
      mStreet(*ui->addOrderTabStreet),
      mTotalPrice(*ui->addOrderTabTotalPrice),
      mFuelTypeBox(*ui->addOrderTabFuelTypeBox),
      mDatabaseApi(databaseApi),
      mChoosenFuelType(Common::FuelType::ERR) {
  connect(ui->addOrderTabAddOrderButton, SIGNAL(pressed()), this,
          SLOT(addOrderPressed()));
  connect(ui->addOrderTabCalculateButton, SIGNAL(pressed()), this,
          SLOT(calculatePressed()));
  connect(ui->addOrderTabFuelTypeBox, SIGNAL(currentIndexChanged(int)), this,
          SLOT(fuelTypeChanged(int)));
}

QString AddOrder::getOrdererName() {
  QString tmp = mOrdererName.text();

  if (not tmp.size()) throw QString("Pole nazwa obiorcy nie moze byc puste!");

  return tmp;
}

uint AddOrder::getAmount() {
  QString tmp = mAmount.text();

  if (not tmp.size()) throw QString("Pole ilość paliwa nie moze byc puste!");

  bool result = true;
  uint amount = tmp.toUInt(&result);

  if (result == false) throw QString("Blędna ilość paliwa!");

  return amount;
}

QString AddOrder::getCity() {
  QString tmp = mCity.text();

  if (not tmp.size()) throw QString("Pole miasto nie moze byc puste!");

  return tmp;
}

uint AddOrder::getIncome() {
  QString tmp = mIncome.text();

  if (not tmp.size()) throw QString("Pole planowany zysk nie moze byc puste!");

  bool result = true;
  uint income = tmp.toUInt(&result);

  if (result == false) throw QString("Błędna wartość w polu planowany zysk!");

  return income;
}

uint AddOrder::getNumber() {
  QString tmp = mNumber.text();

  if (not tmp.size()) throw QString("Pole numer posesji nie moze byc puste!");

  bool result = true;
  uint number = tmp.toUInt(&result);

  if (result == false) throw QString("Błędna wartość w polu numer posesji!");

  return number;
}

QString AddOrder::getStreet() {
  QString tmp = mStreet.text();

  if (not tmp.size()) throw QString("Pole ulica nie moze byc puste!");

  return tmp;
}

void AddOrder::calculatePressed() {
  try {
    getAmount();
    getCity();
    getIncome();
    getNumber();
    getOrdererName();
    getStreet();

  } catch (QString &e) {
    QMessageBox msgBox;
    msgBox.setText(e);
    msgBox.exec();
  }

  qDebug("calculatePressed");
}

void AddOrder::addOrderPressed() { qDebug("addOrderPressed"); }

void AddOrder::fuelTypeChanged(int typeID) {
  qDebug("FuelType changed to %d", typeID);
  switch (typeID) {
    case 0:
      mChoosenFuelType = Common::FuelType::ERR;
      break;
    case 1:
      mChoosenFuelType = Common::FuelType::ON;
      break;
    case 2:
      mChoosenFuelType = Common::FuelType::OO;
      break;
    default:
      mChoosenFuelType = Common::FuelType::ERR;
  }
}
}  // namespace BackEnd
