
#include "Orders.h"
#include <QCalendarWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include "DataBaseApi.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

Orders::Orders(Ui::MainWindow *ui, DataBaseApi &dataBaseApi)
    : mOrders(*ui->ordersTabOrdersTable),
      mWyswietlButton(*ui->ordersTabWyswietlButton),
      mCalendar(*ui->ordersTabCalendar),
      mAdditionalInformation(*ui->ordersTabAdditionalInfoText),
      mDataBaseApi(dataBaseApi) {
  connect(ui->ordersTabWyswietlButton, SIGNAL(pressed()), this,
          SLOT(wyswietlPressed()));
  connect(ui->ordersTabCalendar, SIGNAL(selectionChanged()), this,
          SLOT(chosenDateChanged()));
}

void Orders::printEmptyTable(void) {
  int rowCnt = 0;
  int columnCnt = 0;

  mOrders.insertRow(rowCnt);
  mOrders.insertColumn(columnCnt);

  mOrders.setItem(rowCnt, columnCnt, new QTableWidgetItem("pierwszy"));

  mOrders.insertColumn(++columnCnt);
  mOrders.setItem(rowCnt, columnCnt, new QTableWidgetItem("drugi"));
}

void Orders::wyswietlPressed() { qDebug("wystiwetl preseses"); }

void Orders::chosenDateChanged() {
  mSelectedDate = mCalendar.selectedDate();

  qDebug("chosenDateChanged pressed %d:%d:%d", mSelectedDate.day(),
         mSelectedDate.month(), mSelectedDate.year());
}
