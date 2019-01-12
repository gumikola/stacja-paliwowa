#include "AddProduct.h"
#include "DataBase/DataBaseApi.h"
#include <QMessageBox>

namespace BackEnd {

AddProduct::AddProduct(DataBaseApi::DataBaseApi& databaseApi)
    : mUi(new Ui::AddProduct)
    , mDatabaseApi(databaseApi)
{
    mUi->setupUi(&mDialog);
    connect(mUi->addButton, SIGNAL(pressed()), this, SLOT(addPressed()));
    connect(mUi->cancelButton, SIGNAL(pressed()), this, SLOT(cancelPressed()));
    mDialog.open();
}

void AddProduct::exec()
{
    mDialog.exec();
}

void AddProduct::addPressed()
{
    qDebug("%s", __PRETTY_FUNCTION__);
    try
    {
        QString tmp = getProductName();

        mDatabaseApi.addProduct(tmp);
        emit productAdded(tmp);

        mDialog.close();
    }
    catch (QString& e)
    {
        QMessageBox msgBox;
        msgBox.setFont(QFont(QString("Arial"), 14));
        msgBox.setText(e);
        msgBox.exec();
    }
}

void AddProduct::cancelPressed()
{
    mDialog.close();
}

QString AddProduct::getProductName()
{
    QString tmp = mUi->productName->text();

    if (not tmp.size())
        throw QString("Pole nazwa produktu nie moze byc puste!");

    return tmp;
}

} // namespace BackEnd
