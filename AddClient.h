#pragma once

#include "Common.h"
#include "ui_add_client.h"
#include <QObject>

namespace DataBaseApi {
class DataBaseApi;
} // namespace DataBaseApi

namespace BackEnd {

class AddClient : public QObject
{
    Q_OBJECT

    Ui::AddClient*            mUi;
    DataBaseApi::DataBaseApi& mDatabaseApi;
    QDialog                   mDialog;

    QString getName();
    QString getCity();
    QString getStreet();
    QString getPropertyNumber();

public:
    explicit AddClient(DataBaseApi::DataBaseApi& databaseApi);
    void exec();

signals:

public slots:
    void addPressed();
    void cancelPressed();

signals:
    void clientAdded(Common::CustomerStruct& client);
};
} // namespace BackEnd
