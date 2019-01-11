#pragma once

#include "Common.h"
#include "ui_edit_client.h"
#include <QObject>

namespace DataBaseApi {
class DataBaseApi;
} // namespace DataBaseApi

namespace BackEnd {

class EditClient : public QObject
{
    Q_OBJECT

    Ui::EditClient*               mUi;
    DataBaseApi::DataBaseApi&     mDatabaseApi;
    QDialog                       mDialog;
    const Common::CustomerStruct& mCustomerData;

public:
    explicit EditClient(DataBaseApi::DataBaseApi& databaseApi, const Common::CustomerStruct& data);
    void exec();

private:
    QString getName();
    QString getCity();
    QString getStreet();
    QString getPropertyNumber();

public slots:
    void savePressed();
    void cancelPressed();

signals:
    void clientAdded(Common::CustomerStruct& client);
};

} // namespace BackEnd
