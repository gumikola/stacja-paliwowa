#include "SuggestedProducts.h"
#include "AddClient.h"
#include "ChooseCustomer.h"
#include "Common.h"
#include "DataBase/DataBaseApi.h"

namespace BackEnd {

SuggestedProducts::SuggestedProducts(Ui::MainWindow* ui, DataBaseApi::DataBaseApi& databaseApi)
    : mDatabaseApi(databaseApi)
    , mClientTable(*ui->SuggestedProductsTabChoosenClientTable)
    , mProductsTable(*ui->SuggestedProductsTabProductsTable)
{

    connect(ui->SuggestedProductsTabChooseClientFromDatabase, SIGNAL(pressed()), this,
            SLOT(chooseCustomerPressed()));
    connect(ui->SuggestedProductsTabAddNewClient, SIGNAL(pressed()), this,
            SLOT(addNewCustomerPressed()));
    connect(ui->OptionsTab, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    printDefaultClientTable();
    printDefaultProductsTable();
}

void SuggestedProducts::chooseCustomerPressed()
{
    ChooseCustomer window(mDatabaseApi);
    connect(&window, SIGNAL(clientChosed(const Common::CustomerStruct&)), this,
            SLOT(clientChoosed(const Common::CustomerStruct&)));
    window.exec();
}

void SuggestedProducts::clientChoosed(const Common::CustomerStruct& client)
{
    int rowCnt = 0;
    mClientTable.setRowCount(rowCnt);
    int columnCnt = 0;
    mClientTable.insertRow(rowCnt);
    mClientTable.setVerticalHeaderItem(rowCnt, new QTableWidgetItem());
    mClientTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.name));
    mClientTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.city));
    mClientTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.street));
    mClientTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.propertyNumber));
}

void SuggestedProducts::addNewCustomerPressed()
{
    AddClient window(mDatabaseApi);
    connect(&window, SIGNAL(clientAdded(const Common::CustomerStruct&)), this,
            SLOT(clientChoosed(const Common::CustomerStruct&)));
    window.exec();
}

void SuggestedProducts::tabChanged(int id)
{
    if (id == 4)
    {
        printDefaultClientTable();
        printDefaultProductsTable();
    }
}

void SuggestedProducts::printDefaultClientTable()
{
    mProductsTable.setSelectionMode(QAbstractItemView::NoSelection);
    mClientTable.setContextMenuPolicy(Qt::CustomContextMenu);

    int columnCnt = 0;
    mClientTable.setFont(QFont(QString("Arial"), 14));

    mClientTable.setColumnCount(columnCnt);
    mClientTable.setRowCount(0);

    mClientTable.insertColumn(columnCnt);
    mClientTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Imie i nazwisko"));

    mClientTable.insertColumn(columnCnt);
    mClientTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Miasto"));

    mClientTable.insertColumn(columnCnt);
    mClientTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Ulica"));

    mClientTable.insertColumn(columnCnt);
    mClientTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Numer"));

    for (int i = 0; i < columnCnt; i++)
    {
        if (i == 0)
            mClientTable.setColumnWidth(i, mClientTable.width() / 3 - 20);
        else if (i == 3)
            mClientTable.setColumnWidth(i, 70);
        else
            mClientTable.setColumnWidth(i, (2 * mClientTable.width() / 3 - 70) / (columnCnt - 2));
    }
}

void SuggestedProducts::printDefaultProductsTable()
{
    mProductsTable.setSelectionMode(QAbstractItemView::NoSelection);
    mProductsTable.setContextMenuPolicy(Qt::CustomContextMenu);

    int columnCnt = 0;
    mProductsTable.setFont(QFont(QString("Arial"), 14));

    mProductsTable.setColumnCount(columnCnt);
    mProductsTable.setRowCount(0);

    mProductsTable.insertColumn(columnCnt);
    mProductsTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Numer\nproduktu"));

    mProductsTable.insertColumn(columnCnt);
    mProductsTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Nazwa produktu"));

    for (int i = 0; i < columnCnt; i++)
    {
        if (i == 1)
            mProductsTable.setColumnWidth(i, 3 * mProductsTable.width() / 4 - 16);
        else
            mProductsTable.setColumnWidth(i, (mProductsTable.width() / 4) / (columnCnt - 1));
    }
}

} // namespace BackEnd
