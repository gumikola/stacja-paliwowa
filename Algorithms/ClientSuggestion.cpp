#include "ClientSuggestion.h"
#include <algorithm>

namespace Algorithms {

ClientSuggestion::ClientSuggestion(const QVector<Common::PurchaseStruct>& clientPurchases)
{
    for (Common::PurchaseStruct purchase : clientPurchases)
    {
        QList<PurchaseStruct>::iterator it = std::find_if(
            mPurchases.begin(), mPurchases.end(), [&purchase](const PurchaseStruct& item) {
                return purchase.nameOfProduct == item.purchase.nameOfProduct;
            });

        if (it != mPurchases.end())
        {
            it->count++;
        }
        else
        {
            PurchaseStruct tmp;
            tmp.count     = 1;
            tmp.purchase = purchase;
            mPurchases.push_back(tmp);
        }
    }
}

QVector<Common::PurchaseStruct> ClientSuggestion::getSuggestedProducts(uint maxNumberOfProducts)
{
    QVector<Common::PurchaseStruct> sugestedProducts;

    std::sort(
        mPurchases.begin(), mPurchases.end(),
        [](const PurchaseStruct& p1, const PurchaseStruct& p2) { return p1.count > p2.count; });
    uint cnt = 0;
    for (PurchaseStruct purchase : mPurchases)
    {
        cnt++;
        if (cnt > maxNumberOfProducts)
            return sugestedProducts;

        sugestedProducts.push_back(purchase.purchase);
    }

    return sugestedProducts;
}
} // namespace Algorithms
