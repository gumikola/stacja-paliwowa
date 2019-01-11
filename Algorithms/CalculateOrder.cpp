#include "CalculateOrder.h"
namespace Algorithms {

CalculateOrder::CalculateOrder(const QVector<Common::OrdersStruct> Customers)
    : mCustomers(Customers)
{
}
void CalculateOrder::GetCustomersID()
{
    for (int i = 0; i < mCustomers.size(); i++)
    {
        //        mIds.append(DataBaseApi::DataBaseApi::GetCustomerID(mCustomers.at(i)));
    }
}

void CalculateOrder::checkIfDistanceIsInDatabase()
{
    QVector<Common::DistancesStruct> allDistancesFromDatabase;
    //   = DataBaseApi::DataBaseApi::GetAllDistances();
    for (int i = 0; i < allDistancesFromDatabase.size(); i++)
    {
        for (int j = 0; j < mIds.size(); j++)
        {
            for (int k = 0; j + 1 < mIds.size(); k++)
            {
                if ((allDistancesFromDatabase.at(i).a == mIds.at(j)))
                {
                    ;
                }
            }
        }
    }
}

void CalculateOrder::CheckDistancesInDatabase()
{
    //    //    QVector<Common::CustomerStruct> customersToCompare;
    //    QVector<Common::CustomerStruct> distancesFromDatabese;
    //    //    const fromdatabase = get
    //    for (int i = 0; i < mCustomers.size(); i++)
    //    {
    //        for (int j = i + 1; j < mCustomers.size(); j++)
    //        {
    //            if (QueryDatabaseIfDistanceIsIn(fromdatabase, mCustomers.ar(i), mCustomers.at(j)))
    //                ; // add to vector vector
    //            else
    //                ; // add to database and vector vector

    //            //            customersToCompare.clear();
    //            //            customersToCompare.append(mCustomers[i].customer);
    //            //            customersToCompare.append(mCustomers[j].customer);
    //            //            if (!QueryDatabaseIfDistanceIsIn(customersToCompare))
    //            //            {
    //            //                customersToGetDistance.append(customersToCompare);
    //            //            }
    //            //            if (customersToGetDistance.size() ==)
    //        }
    //    }
}

} // namespace Algorithms
