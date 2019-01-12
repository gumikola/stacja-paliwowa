#include "AddClientPurchase.h"
#include "DataBase/DataBaseApi.h"
#include <QDate>
#include <QMessageBox>

namespace BackEnd {

AddClientPurchase::AddClientPurchase(DataBaseApi::DataBaseApi&     databaseApi,
                                     const Common::CustomerStruct& customer)
    : mUi(new Ui::AddClientPurchase)
    , mDatabaseApi(databaseApi)
    , mCustomer(customer)
{
    mUi->setupUi(&mDialog);
    connect(mUi->addPurchaseButton, SIGNAL(pressed()), this, SLOT(addPurchasePressed()));
    connect(mUi->cancelButton, SIGNAL(pressed()), this, SLOT(cancelPressed()));
    connect(mUi->nameFilter, SIGNAL(textChanged(const QString&)), this,
            SLOT(filterChanged(const QString&)));
    mDialog.open();
    printDefaultTable();
    printFiltredProducts(QString());
}

void AddClientPurchase::exec()
{
    mDialog.exec();
}

void AddClientPurchase::addPurchasePressed()
{
    int row = mUi->table->currentRow();

    if (row < 0)
    {
        QMessageBox msgBox;
        msgBox.setFont(QFont(QString("Arial"), 14));
        msgBox.setText("Nie wybrano produktu!");
        msgBox.exec();
        return;
    }

    Common::PurchaseStruct purchase;
    purchase.date          = QDate::currentDate();
    purchase.nameOfProduct = mUi->table->item(row, 1)->text();

    mDatabaseApi.addPurchase(purchase, mCustomer);
    mDialog.close();
}

void AddClientPurchase::cancelPressed()
{
    mDialog.close();
}

void AddClientPurchase::filterChanged(const QString& filter)
{
    printFiltredProducts(filter);
}

void AddClientPurchase::printDefaultTable()
{
    mUi->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    mUi->table->setSelectionMode(QAbstractItemView::SingleSelection);
    mUi->table->setContextMenuPolicy(Qt::CustomContextMenu);

    int columnCnt = 0;
    mUi->table->setFont(QFont(QString("Arial"), 14));

    mUi->table->setColumnCount(columnCnt);
    mUi->table->setRowCount(0);

    mUi->table->insertColumn(columnCnt);
    mUi->table->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Numer\nproduktu"));

    mUi->table->insertColumn(columnCnt);
    mUi->table->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Nazwa produktu"));

    for (int i = 0; i < columnCnt; i++)
    {
        if (i == 1)
            mUi->table->setColumnWidth(i, 2 * mUi->table->width() / 3 - 16);
        else
            mUi->table->setColumnWidth(i, (mUi->table->width() / 3) / (columnCnt - 1));
    }
}

void AddClientPurchase::printFiltredProducts(const QString& filter)
{
    QStringList products = mDatabaseApi.getProuducts();

    int rowCnt = 0;
    mUi->table->setRowCount(rowCnt);

    for (QString productName : products)
    {
        if (productName.contains(
                QRegularExpression(filter, QRegularExpression::CaseInsensitiveOption)))
        {
            int columnCnt = 0;
            mUi->table->insertRow(rowCnt);
            mUi->table->setVerticalHeaderItem(rowCnt, new QTableWidgetItem());
            mUi->table->setItem(rowCnt, columnCnt++,
                                new QTableWidgetItem(QString::number(rowCnt + 1)));
            mUi->table->setItem(rowCnt, columnCnt++, new QTableWidgetItem(productName));
            rowCnt++;
        }
    }
}

} // namespace BackEnd
