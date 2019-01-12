#pragma once

#include "Common.h"
#include "ui_mainwindow.h"
#include <QObject>

namespace DataBaseApi {
class DataBaseApi;
} // namespace DataBaseApi

namespace BackEnd {

class SuggestedProducts : public QObject
{
    Q_OBJECT
    DataBaseApi::DataBaseApi& mDatabaseApi;
    QTableWidget&             mClientTable;
    QTableWidget&             mProductsTable;
    Common::CustomerStruct    mChosenCustomer;

    void printDefaultClientTable();
    void printDefaultProductsTable();
    void printSuggestedProducts();

public:
    explicit SuggestedProducts(Ui::MainWindow* ui, DataBaseApi::DataBaseApi& databaseApi);

signals:

public slots:
    void chooseCustomerPressed();
    void clientChoosed(const Common::CustomerStruct& client);
    void addNewCustomerPressed();
    void tabChanged(int id);
    void addNewPurchasePressed();
};

} // namespace BackEnd
