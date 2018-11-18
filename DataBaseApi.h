#ifndef DATABASEAPI_H
#define DATABASEAPI_H
#include <QDate>
#include <QMap>

class DataBaseApi {
 public:
  DataBaseApi();

  QMap<QString, QMap<QString, uint32_t>> getTanksFillLevel();
};

#endif  // DATABASEAPI_H
