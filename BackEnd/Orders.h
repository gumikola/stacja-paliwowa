#ifndef ORDERS_H
#define ORDERS_H

#include <QCalendarWidget>
#include <QMainWindow>
#include <QObject>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include "DataBase/DataBaseApi.h"

#include "ui_mainwindow.h"

class Orders : QObject {
  Q_OBJECT
  QTableWidget &mOrders;
  QPushButton &mWyswietlButton;
  QCalendarWidget &mCalendar;
  QTextEdit &mAdditionalInformation;
  DataBaseApi mDataBaseApi;

  QDate mSelectedDate;

 public:
  Orders(Ui::MainWindow *ui, DataBaseApi &dataBaseApi);

  void printEmptyTable(void);

 private slots:
  void wyswietlPressed();
  void chosenDateChanged();
};

#endif  // ORDERS_H
