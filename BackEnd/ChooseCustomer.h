#pragma once

#include "Common.h"
#include "ui_choose_customer.h"
#include <QObject>

namespace DataBaseApi {
class DataBaseApi;
} // namespace DataBaseApi

namespace BackEnd {

class ChooseCustomer : public QObject
{
    Q_OBJECT
    Ui::ChooseCustomer*       mUi;
    DataBaseApi::DataBaseApi& mDatabaseApi;
    QDialog                   mDialog;

public:
    explicit ChooseCustomer(DataBaseApi::DataBaseApi& databaseApi);
    void exec();

private:
    void printDefaultTable(void);
    void printFiltredClients(const QString& filter);

public:
signals:
    void clientChosed(const Common::CustomerStruct& client);

public slots:
    void addPressed();
    void cancelPressed();
    void filterChanged(const QString&);
};

} // namespace BackEnd
