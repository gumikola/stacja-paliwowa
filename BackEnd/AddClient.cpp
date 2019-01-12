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
    QString tmp = mUi->name->text();

    if (not tmp.size())
        throw QString("Pole imie i nazwisko nie moze byc puste!");

    return tmp;
}

QString AddClient::getCity()
{
    QString tmp = mUi->city->text();

    if (not tmp.size())
        throw QString("Pole miasto nie moze byc puste!");

    return tmp;
}

QString AddClient::getStreet()
{
    QString tmp = mUi->street->text();

    if (not tmp.size())
        throw QString("Pole ulica nie moze byc puste!");

    return tmp;
}

QString AddClient::getPropertyNumber()
{
    QString tmp = mUi->number->text();

    if (not tmp.size())
        throw QString("Pole numer nie moze byc puste!");

    return tmp;
}

void AddClient::exec()
{
    mDialog.exec();
}

void AddClient::addPressed()
{
    qDebug("addPressed()");
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
    mDialog.close();
}

} // namespace BackEnd
