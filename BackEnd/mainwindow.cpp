#include "mainwindow.h"
#include <QMap>
#include <QScopedPointer>
#include "AddOrder.h"
#include "Orders.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::makeObjects() {
  // first set fill level of fuel tanks
  QMap<QString, QMap<QString, uint32_t>> fuelTanksFillLevel =
      mDataBaseApi.getTanksFillLevel();

  // #TO_DO

  // making objects
  mOrders.reset(new BackEnd::Orders(ui, mDataBaseApi));
  mAddOrder.reset(new BackEnd::AddOrder(ui, mDataBaseApi));
}
