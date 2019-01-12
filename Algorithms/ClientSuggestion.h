#pragma once

#include "Common.h"
#include <QMap>
#include <QString>
#include <QVector>

namespace Algorithms {

struct Purcharse
{
    Common::PurchaseStruct purcharse;
    uint                   count;
};

class ClientSuggestion
{
    QList<Purcharse> mPurcharses;

public:
    ClientSuggestion(const QVector<Common::PurchaseStruct>& mClientPurchases);
    QVector<Common::PurchaseStruct> GetSuggestedProducts(uint maxNumberOfProducts);
};
} // namespace Algorithms
