#ifndef ADDORDER_H
#define ADDORDER_H

#include <QComboBox>
#include <QLineEdit>
#include <QMainWindow>
#include <QObject>
#include <QPushButton>
#include <QString>
#include <QTableWidget>
#include <QTextEdit>
#include "Common.h"
#include "DataBase/DataBaseApi.h"
#include "ui_mainwindow.h"

namespace BackEnd {

class AddOrder : public QObject {
  Q_OBJECT

  QPushButton &mAddOrderButton;
  QPushButton &mCalculateButton;
  QLineEdit &mAmount;
  QLineEdit &mCity;
  QLineEdit &mIncome;
  QLineEdit &mMargin;
  QLineEdit &mNumber;
  QLineEdit &mOrdererName;
  QLineEdit &mStreet;
  QLineEdit &mTotalPrice;
  QComboBox &mFuelTypeBox;
  DataBaseApi::DataBaseApi mDatabaseApi;
  Common::FuelType mChoosenFuelType;

  QString getOrdererName();
  uint getAmount();
  QString getCity();
  uint getIncome();
  uint getNumber();
  QString getStreet();

 public:
  AddOrder(Ui::MainWindow *ui, DataBaseApi::DataBaseApi &databaseApi);

 signals:

 public slots:

  void calculatePressed();
  void addOrderPressed();
  void fuelTypeChanged(int);
};

}  // namespace BackEnd

#endif  // ADDORDER_H
