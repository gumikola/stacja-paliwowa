#include "ClientsTab.h"
#include "AddClient.h"
#include "DataBase/DataBaseApi.h"
#include "EditClient.h"
#include <ui_mainwindow.h>

namespace BackEnd {

ClientsTab::ClientsTab(Ui::MainWindow* ui, DataBaseApi::DataBaseApi& databaseApi)
    : mUi(ui)
    , mDatabaseApi(databaseApi)
    , mTable(*ui->ClientsTabClientsTable)
{
    connect(ui->ClientsTabAddButton, SIGNAL(pressed()), this, SLOT(addPressed()));
    connect(ui->ClientsTabClientsTable, SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(displayMenu(QPoint)));
    connect(ui->OptionsTab, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    connect(mUi->ClientsTabSurnameFiltr, SIGNAL(textChanged(const QString&)), this,
            SLOT(filterChanged(const QString&)));

    printDefaultTable();
    printClients();
}

void ClientsTab::printDefaultTable()
{
    qDebug() << __PRETTY_FUNCTION__;

    mTable.setSelectionBehavior(QAbstractItemView::SelectRows);
    mTable.setSelectionMode(QAbstractItemView::SingleSelection);
    mTable.setContextMenuPolicy(Qt::CustomContextMenu);

    int columnCnt = 0;
    mTable.setFont(QFont(QString("Arial"), 14));

    mTable.setColumnCount(columnCnt);
    mTable.setRowCount(0);

    mTable.insertColumn(columnCnt);
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Imie i nazwisko"));

    mTable.insertColumn(columnCnt);
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Miasto"));

    mTable.insertColumn(columnCnt);
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Ulica"));

    mTable.insertColumn(columnCnt);
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Numer"));

    for (int i = 0; i < columnCnt; i++)
    {
        if (i == 0)
            mTable.setColumnWidth(i, mTable.width() / 3 - 24);
        else if (i == 3)
            mTable.setColumnWidth(i, 80);
        else
            mTable.setColumnWidth(i, (2 * mTable.width() / 3 - 80) / (columnCnt - 2));
    }
}

void ClientsTab::printFiltredClients(const QString& filter)
{
    qDebug() << __PRETTY_FUNCTION__;

    QVector<Common::CustomerStruct> clients = mDatabaseApi.getCustomers();
    int                             rowCnt  = 0;
    mTable.setRowCount(0);
    for (Common::CustomerStruct client : clients)
    {
        if (client.name.contains(
                QRegularExpression(filter, QRegularExpression::CaseInsensitiveOption)))
        {
            int columnCnt = 0;
            mTable.insertRow(rowCnt);
            mTable.setVerticalHeaderItem(rowCnt, new QTableWidgetItem());

            mTable.setItem(rowCnt, columnCnt, new QTableWidgetItem(client.name));
            mTable.item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignCenter);

            mTable.setItem(rowCnt, columnCnt, new QTableWidgetItem(client.city));
            mTable.item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignCenter);

            mTable.setItem(rowCnt, columnCnt, new QTableWidgetItem(client.street));
            mTable.item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignCenter);

            mTable.setItem(rowCnt, columnCnt, new QTableWidgetItem(client.propertyNumber));
            mTable.item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignCenter);

            rowCnt++;
        }
    }
}

void ClientsTab::printClients()
{
    qDebug() << __PRETTY_FUNCTION__;

    QVector<Common::CustomerStruct> clients = mDatabaseApi.getCustomers();
    int                             rowCnt  = 0;
    mTable.setRowCount(0);
    for (Common::CustomerStruct client : clients)
    {

        int columnCnt = 0;
        mTable.insertRow(rowCnt);
        mTable.setVerticalHeaderItem(rowCnt, new QTableWidgetItem());

        mTable.setItem(rowCnt, columnCnt, new QTableWidgetItem(client.name));
        mTable.item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignCenter);

        mTable.setItem(rowCnt, columnCnt, new QTableWidgetItem(client.city));
        mTable.item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignCenter);

        mTable.setItem(rowCnt, columnCnt, new QTableWidgetItem(client.street));
        mTable.item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignCenter);

        mTable.setItem(rowCnt, columnCnt, new QTableWidgetItem(client.propertyNumber));
        mTable.item(rowCnt, columnCnt++)->setTextAlignment(Qt::AlignCenter);

        rowCnt++;
    }
}

void ClientsTab::addPressed()
{
    qDebug() << __PRETTY_FUNCTION__;

    AddClient window(mDatabaseApi);
    window.exec();
}

void ClientsTab::displayMenu(QPoint pos)
{
    qDebug() << __PRETTY_FUNCTION__;

    QMenu menu(&(mTable));

    QAction* addNewClient = menu.addAction("Dodaj nowego klienta");
    QAction* edit         = menu.addAction("Edytuj");
    QAction* remove       = menu.addAction("Usun");

    connect(addNewClient, SIGNAL(triggered()), this, SLOT(addPressed()));
    connect(edit, SIGNAL(triggered()), this, SLOT(editPressed()));
    connect(remove, SIGNAL(triggered()), this, SLOT(removePressed()));

    menu.exec(mTable.viewport()->mapToGlobal(pos));
}

void ClientsTab::tabChanged(int id)
{
    if (id == 3)
    {
        qDebug() << __PRETTY_FUNCTION__;
        printDefaultTable();
        printClients();
        mUi->ClientsTabSurnameFiltr->clear();
    }
}

void ClientsTab::filterChanged(const QString& text)
{
    qDebug() << __PRETTY_FUNCTION__ << " filtr=" << text;

    printFiltredClients(text);
}

void ClientsTab::removePressed()
{
    int row = mTable.currentRow();

    qDebug() << __PRETTY_FUNCTION__ << " row=" << row;

    if (row >= 0)
    {

        Common::CustomerStruct tmp;
        tmp.name           = mTable.item(row, 0)->text();
        tmp.city           = mTable.item(row, 1)->text();
        tmp.street         = mTable.item(row, 2)->text();
        tmp.propertyNumber = mTable.item(row, 3)->text();

        mDatabaseApi.removeCustomer(tmp);
        printClients();
    }
}

void ClientsTab::editPressed()
{
    int row = mTable.currentRow();

    qDebug() << __PRETTY_FUNCTION__ << " row=" << row;

    if (row >= 0)
    {
        Common::CustomerStruct tmp;
        tmp.name           = mTable.item(row, 0)->text();
        tmp.city           = mTable.item(row, 1)->text();
        tmp.street         = mTable.item(row, 2)->text();
        tmp.propertyNumber = mTable.item(row, 3)->text();

        EditClient window(mDatabaseApi, tmp);
        window.exec();

        printClients();
    }
}
} // namespace BackEnd
