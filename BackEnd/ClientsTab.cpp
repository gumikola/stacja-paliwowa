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
            mTable.setColumnWidth(i, mTable.width() / 2 - 16);
        else
            mTable.setColumnWidth(i, (mTable.width() / 2) / (columnCnt - 1));
    }
}

void ClientsTab::printFiltredClients(const QString& filter)
{

    QVector<Common::CustomerStruct> clients = mDatabaseApi.getClients();
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
            mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.name));
            mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.city));
            mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.street));
            mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.propertyNumber));

            rowCnt++;
        }
    }
}

void ClientsTab::printClients()
{
    QVector<Common::CustomerStruct> clients = mDatabaseApi.getClients();
    int                             rowCnt  = 0;
    mTable.setRowCount(0);
    for (Common::CustomerStruct client : clients)
    {

        int columnCnt = 0;
        mTable.insertRow(rowCnt);
        mTable.setVerticalHeaderItem(rowCnt, new QTableWidgetItem());
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.name));
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.city));
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.street));
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.propertyNumber));

        rowCnt++;
    }
}

void ClientsTab::addPressed()
{
    AddClient window(mDatabaseApi);
    window.exec();
}

void ClientsTab::displayMenu(QPoint pos)
{
    qDebug("display additional menu pressed");
    QMenu menu(&(mTable));

    QAction* remove = menu.addAction("Usun");
    QAction* edit   = menu.addAction("Edytuj");
    QAction* a      = menu.exec(mTable.viewport()->mapToGlobal(pos));
    if (a == remove)
    {
        int row = mTable.currentRow();
        qDebug("remove choosed row index=%d", row); // check if index is higher then 0
                                                    // if table is empty index is -1
        if (row >= 0)
        {

            Common::CustomerStruct tmp;
            tmp.name           = mTable.item(row, 0)->text();
            tmp.city           = mTable.item(row, 1)->text();
            tmp.street         = mTable.item(row, 2)->text();
            tmp.propertyNumber = mTable.item(row, 3)->text();

            mDatabaseApi.removeClient(tmp);
            printClients();
        }
    }
    if (a == edit)
    {
        int row = mTable.currentRow();
        qDebug("edit choosed row index=%d", row);
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

void ClientsTab::tabChanged(int)
{
    printDefaultTable();
    printClients();
    mUi->ClientsTabSurnameFiltr->clear();
}

void ClientsTab::filterChanged(const QString& text)
{
    qDebug("%s zmieniono na: %s", __func__, text.toStdString().c_str());
    printFiltredClients(text);
}
} // namespace BackEnd
