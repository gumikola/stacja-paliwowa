#include "AddClient.h"
#include "DataBase/DataBaseApi.h"
#include <QMessageBox>

namespace BackEnd {

AddClient::AddClient(DataBaseApi::DataBaseApi& databaseApi)
    : mUi(new Ui::AddClient)
    , mDatabaseApi(databaseApi)
{
    mUi->setupUi(&mDialog);
    connect(mUi->addButton, SIGNAL(pressed()), this, SLOT(addPressed()));
    connect(mUi->cancelButton, SIGNAL(pressed()), this, SLOT(cancelPressed()));
    mDialog.open();
}

QString AddClient::getName()
{
    qDebug() << __PRETTY_FUNCTION__;

    QString tmp = mUi->name->text();

    if (not tmp.size())
        throw QString("Pole imie i nazwisko nie moze byc puste!");

    return tmp;
}

QString AddClient::getCity()
{
    qDebug() << __PRETTY_FUNCTION__;

    QString tmp = mUi->city->text();

    if (not tmp.size())
        throw QString("Pole miasto nie moze byc puste!");

    return tmp;
}

QString AddClient::getStreet()
{
    qDebug() << __PRETTY_FUNCTION__;

    QString tmp = mUi->street->text();

    if (not tmp.size())
        throw QString("Pole ulica nie moze byc puste!");

    return tmp;
}

QString AddClient::getPropertyNumber()
{
    qDebug() << __PRETTY_FUNCTION__;

    QString tmp = mUi->number->text();

    if (not tmp.size())
        throw QString("Pole numer nie moze byc puste!");

    return tmp;
}

void AddClient::exec()
{
    qDebug() << __PRETTY_FUNCTION__;

    mDialog.exec();
}

void AddClient::addPressed()
{
    qDebug() << __PRETTY_FUNCTION__;

    qDebug("%s", __PRETTY_FUNCTION__);
    try
    {
        Common::CustomerStruct tmp;
        tmp.name           = getName();
        tmp.city           = getCity();
        tmp.street         = getStreet();
        tmp.propertyNumber = getPropertyNumber();

        mDatabaseApi.addCustomer(tmp);
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

void AddClient::cancelPressed()
{
    qDebug() << __PRETTY_FUNCTION__;

    mDialog.close();
}

} // namespace BackEnd
