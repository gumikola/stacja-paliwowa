#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>
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
  QScopedPointer<Orders> mOrders;
};

#endif  // MAINWINDOW_H
