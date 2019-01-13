#pragma once

#include "Common.h"
#include "ui_mainwindow.h"
#include <QObject>

namespace DataBaseApi {
class DataBaseApi;
} // namespace DataBaseApi

namespace BackEnd {

class ClientsTab : public QObject
{
    Q_OBJECT
    Ui::MainWindow*           mUi;
    DataBaseApi::DataBaseApi& mDatabaseApi;
    QTableWidget&             mTable;

    void printDefaultTable(void);
    void printFiltredClients(const QString& filter);
    void printClients();

public:
    explicit ClientsTab(Ui::MainWindow* mUi, DataBaseApi::DataBaseApi& mDatabaseApi);

signals:

public slots:
    void addPressed();
    void displayMenu(QPoint pos);
    void tabChanged(int id);
    void filterChanged(const QString& text);
    void removePressed();
    void editPressed();
};

} // namespace BackEnd
