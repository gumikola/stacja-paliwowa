#include "ClientSuggestion.h"
#include <algorithm>

namespace Algorithms {

ClientSuggestion::ClientSuggestion(const QVector<Common::PurchaseStruct>& clientPurchases)
{
    for (Common::PurchaseStruct purcharse : clientPurchases)
    {
        auto it = std::find_if(mPurcharses.begin(), mPurcharses.end(),
                               [&purcharse](const Purcharse& item) {
                                   return purcharse.nameOfProduct == item.purcharse.nameOfProduct;
                               });

        if (it != mPurcharses.end())
        {
            it->count++;
        }
        else
        {
            it->purcharse = purcharse;
            it->count     = 1;
        }
    }
}

QVector<Common::PurchaseStruct> ClientSuggestion::GetSuggestedProducts(uint maxNumberOfProducts)
{
    QVector<Common::PurchaseStruct> sugestedProducts;

    std::sort(mPurcharses.begin(), mPurcharses.end(),
              [](const Purcharse& p1, const Purcharse& p2) { return p1.count < p2.count; });
    uint cnt = 0;
    for (Purcharse purcharse : mPurcharses)
    {
        cnt++;
        if (cnt > maxNumberOfProducts)
            return sugestedProducts;

        sugestedProducts.push_back(purcharse.purcharse);
    }

    return sugestedProducts;
}
} // namespace Algorithms
