#ifndef CLIENT_SUGGESTION_H
#define CLIENT_SUGGESTION_H
#include <QMap>
#include <QString>
#include <QVector>

namespace Algorithms {

class ClientSuggestion {
  const QVector<QString> &mPreviousOrderedProductsList;

 public:
  ClientSuggestion(const QVector<QString> &PreviousOrderedProductsList);
  QVector<QString> GetSuggestedProducts(void);
  uint GetNumberOfElemnts(QString);
};
}  // namespace Algorithms
#endif  // CLIENT_SUGGESTION_H
