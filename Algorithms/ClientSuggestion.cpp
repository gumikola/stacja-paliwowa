#include "ClientSuggestion.h"

namespace Algorithms {

ClientSuggestion::ClientSuggestion(
    const QVector<QString> &previousOrderedProductsList)
    : mPreviousOrderedProductsList(previousOrderedProductsList) {}
uint ClientSuggestion::GetNumberOfElemnts(QString key) {
  uint Counter = 0;
  for (int i = 0; i < mPreviousOrderedProductsList.size(); i++) {
    if (mPreviousOrderedProductsList.at(i) == key) {
      Counter++;
    }
  }
  return Counter;
}
QVector<QString> ClientSuggestion::GetSuggestedProducts(void) {
  QVector<QString> SugestedProducts;
  QMap<uint, QString> map;

  for (QString order : mPreviousOrderedProductsList) {
    bool isInMap = false;
    for (QString val : map) {
      if (val == order) {
        isInMap = true;
      }
    }
    if (not isInMap) map.insert(GetNumberOfElemnts(order), order);
  }
  SugestedProducts.push_back(map.last());
  return SugestedProducts;
}
}  // namespace Algorithms
