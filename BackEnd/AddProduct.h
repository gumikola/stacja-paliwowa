#pragma once

#include "Common.h"
#include "ui_add_product.h"
#include <QObject>

namespace DataBaseApi {
class DataBaseApi;
} // namespace DataBaseApi

namespace BackEnd {

class AddProduct : public QObject
{
    Q_OBJECT
    Ui::AddProduct*           mUi;
    DataBaseApi::DataBaseApi& mDatabaseApi;
    QDialog                   mDialog;

    QString getProductName();

public:
    explicit AddProduct(DataBaseApi::DataBaseApi& databaseApi);
    void exec();

signals:
    void productAdded(const QString& productName);

public slots:
    void addPressed();
    void cancelPressed();
};

} // namespace BackEnd
