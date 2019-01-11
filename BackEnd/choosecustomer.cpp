#include "choosecustomer.h"
#include "DataBase/DataBaseApi.h"
#include <QMessageBox>

namespace BackEnd {

ChooseCustomer::ChooseCustomer(DataBaseApi::DataBaseApi& databaseApi)
    : mUi(new Ui::ChooseCustomer)
    , mDatabaseApi(databaseApi)
{
    mUi->setupUi(&mDialog);
    connect(mUi->addButton, SIGNAL(pressed()), this, SLOT(addPressed()));
    connect(mUi->cancelButton, SIGNAL(pressed()), this, SLOT(cancelPressed()));
    connect(mUi->surnameFiltr, SIGNAL(textChanged(const QString&)), this,
            SLOT(filterChanged(const QString&)));
    mDialog.open();

    printDefaultTable();
    printFiltredClients(QString());
}

void ChooseCustomer::printDefaultTable()
{
    mUi->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    mUi->table->setSelectionMode(QAbstractItemView::SingleSelection);
    mUi->table->setContextMenuPolicy(Qt::CustomContextMenu);

    int columnCnt = 0;
    mUi->table->setFont(QFont(QString("Arial"), 14));

    mUi->table->setColumnCount(columnCnt);
    mUi->table->setRowCount(0);

    mUi->table->insertColumn(columnCnt);
    mUi->table->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Imie i nazwisko"));

    mUi->table->insertColumn(columnCnt);
    mUi->table->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Miasto"));

    mUi->table->insertColumn(columnCnt);
    mUi->table->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Ulica"));

    mUi->table->insertColumn(columnCnt);
    mUi->table->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Numer"));

    for (int i = 0; i < columnCnt; i++)
    {
        if (i == 0)
            mUi->table->setColumnWidth(i, mUi->table->width() / 3 - 24);
        else if (i == 3)
            mUi->table->setColumnWidth(i, 70);
        else
            mUi->table->setColumnWidth(i, (2 * mUi->table->width() / 3 - 70) / (columnCnt - 2));
    }
}

void ChooseCustomer::printFiltredClients(const QString& filter)
{
    QVector<Common::CustomerStruct> mClients = mDatabaseApi.getClients();

    int rowCnt = 0;

    mUi->table->setRowCount(0);
    for (Common::CustomerStruct client : mClients)
    {
        if (client.name.contains(
                QRegularExpression(filter, QRegularExpression::CaseInsensitiveOption)))
        {
            int columnCnt = 0;
            mUi->table->insertRow(rowCnt);
            mUi->table->setVerticalHeaderItem(rowCnt, new QTableWidgetItem());
            mUi->table->setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.name));
            mUi->table->setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.city));
            mUi->table->setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.street));
            mUi->table->setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.propertyNumber));

            rowCnt++;
        }
    }
}

void ChooseCustomer::exec()
{
    mDialog.exec();
}

void ChooseCustomer::addPressed()
{
    qDebug("%s", __func__);

    try
    {
        int row = mUi->table->currentRow();

        if (row < 0)
            throw QString("Nie wybrano clienta!");

        Common::CustomerStruct tmp;
        tmp.name           = mUi->table->item(row, 0)->text();
        tmp.city           = mUi->table->item(row, 1)->text();
        tmp.street         = mUi->table->item(row, 2)->text();
        tmp.propertyNumber = mUi->table->item(row, 3)->text();

        emit clientChosed(tmp);
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

void ChooseCustomer::cancelPressed()
{
    mDialog.close();
}

void ChooseCustomer::filterChanged(const QString& text)
{
    qDebug("%s zmieniono na: %s", __func__, text.toStdString().c_str());
    printFiltredClients(text);
}

} // namespace BackEnd
