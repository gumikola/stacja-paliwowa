#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DataBase/DataBaseApi.h"
#include <QMainWindow>
#include <QScopedPointer>

namespace Ui {
class MainWindow;
}

namespace BackEnd {
class Orders;
class FuelPriceChart;
class FuelTanks;
class ClientsTab;
} // namespace BackEnd

class MainWindow : public QMainWindow
{

    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void makeObjects();

private:
    Ui::MainWindow*                         ui;
    DataBaseApi::DataBaseApi                mDataBaseApi;
    QScopedPointer<BackEnd::Orders>         mOrders;
    QScopedPointer<BackEnd::FuelPriceChart> mFuelPriceChart;
    QScopedPointer<BackEnd::FuelTanks>      mFuelTanks;
    QScopedPointer<BackEnd::ClientsTab>     mClientsTab;
};

#endif // MAINWINDOW_H
