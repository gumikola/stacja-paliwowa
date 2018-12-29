#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "AddOrder.h"
#include "DataBase/DataBaseApi.h"
#include "Orders.h"
#include "UpdateTanksFillLevel.h"
#include <QMainWindow>
#include <QScopedPointer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{

    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void makeObjects();

private slots:
    void displayTanksFillLevel(void);

private:
    Ui::MainWindow*                               ui;
    DataBaseApi::DataBaseApi                      mDataBaseApi;
    QScopedPointer<BackEnd::Orders>               mOrders;
    QScopedPointer<BackEnd::AddOrder>             mAddOrder;
    QScopedPointer<BackEnd::UpdateTanksFillLevel> mUpdateTanksFillLevel;
};

#endif // MAINWINDOW_H
