#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>
#include "AddOrder.h"
#include "DataBase/DataBaseApi.h"
#include "Orders.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void makeObjects();

 private:
  Ui::MainWindow *ui;
  DataBaseApi::DataBaseApi mDataBaseApi;
  QScopedPointer<BackEnd::Orders> mOrders;
  QScopedPointer<BackEnd::AddOrder> mAddOrder;
};

#endif  // MAINWINDOW_H
