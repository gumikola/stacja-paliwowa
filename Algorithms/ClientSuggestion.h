#pragma once

#include "Common.h"
#include <QMap>
#include <QString>
#include <QVector>

namespace Algorithms {

struct PurchaseStruct
{
    Common::PurchaseStruct purchase;
    uint                   count;
};

class ClientSuggestion
{
    QList<PurchaseStruct> mPurchases;

public:
    ClientSuggestion(const QVector<Common::PurchaseStruct>& mClientPurchases);
    QVector<Common::PurchaseStruct> getSuggestedProducts(uint maxNumberOfProducts);
};
} // namespace Algorithms
