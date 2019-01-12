#pragma once

#include "Common.h"
#include "ui_add_client_purchase.h"
#include <QObject>

namespace DataBaseApi {
class DataBaseApi;
} // namespace DataBaseApi

namespace BackEnd {

class AddClientPurchase : public QObject
{
    Q_OBJECT
    Ui::AddClientPurchase*        mUi;
    DataBaseApi::DataBaseApi&     mDatabaseApi;
    QDialog                       mDialog;
    const Common::CustomerStruct& mCustomer;

    void printDefaultTable(void);
    void printFiltredProducts(const QString& filter);

public:
    explicit AddClientPurchase(DataBaseApi::DataBaseApi&     databaseApi,
                               const Common::CustomerStruct& customer);
    void exec();

signals:

public slots:
    void addPurchasePressed();
    void cancelPressed();
    void filterChanged(const QString& filter);
};

} // namespace BackEnd
