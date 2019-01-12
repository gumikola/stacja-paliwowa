#include "EditClient.h"
#include "DataBase/DataBaseApi.h"
#include <QMessageBox>

namespace BackEnd {

EditClient::EditClient(DataBaseApi::DataBaseApi& databaseApi, const Common::CustomerStruct& data)
    : mUi(new Ui::EditClient)
    , mDatabaseApi(databaseApi)
    , mCustomerData(data)
{
    mUi->setupUi(&mDialog);
    connect(mUi->saveButton, SIGNAL(pressed()), this, SLOT(savePressed()));
    connect(mUi->cancelButton, SIGNAL(pressed()), this, SLOT(cancelPressed()));

    mUi->city->setText(data.city);
    mUi->name->setText(data.name);
    mUi->number->setText(data.propertyNumber);
    mUi->street->setText(data.street);
    mDialog.open();
}

QString EditClient::getName()
{
    qDebug() << __PRETTY_FUNCTION__;

    QString tmp = mUi->name->text();

    if (not tmp.size())
        throw QString("Pole imie i nazwisko nie moze byc puste!");

    return tmp;
}

QString EditClient::getCity()
{
    qDebug() << __PRETTY_FUNCTION__;

    QString tmp = mUi->city->text();

    if (not tmp.size())
        throw QString("Pole miasto nie moze byc puste!");

    return tmp;
}

QString EditClient::getStreet()
{
    qDebug() << __PRETTY_FUNCTION__;

    QString tmp = mUi->street->text();

    if (not tmp.size())
        throw QString("Pole ulica nie moze byc puste!");

    return tmp;
}

QString EditClient::getPropertyNumber()
{
    qDebug() << __PRETTY_FUNCTION__;

    QString tmp = mUi->number->text();

    if (not tmp.size())
        throw QString("Pole numer nie moze byc puste!");

    return tmp;
}

void EditClient::exec()
{
    qDebug() << __PRETTY_FUNCTION__;

    mDialog.exec();
}

void EditClient::savePressed()
{
    qDebug() << __PRETTY_FUNCTION__;

    try
    {
        Common::CustomerStruct tmp;
        tmp.name           = getName();
        tmp.city           = getCity();
        tmp.street         = getStreet();
        tmp.propertyNumber = getPropertyNumber();

        mDatabaseApi.editCustomer(mCustomerData, tmp);
        emit clientAdded(tmp);

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

void EditClient::cancelPressed()
{
    qDebug() << __PRETTY_FUNCTION__;

    mDialog.close();
}

} // namespace BackEnd
