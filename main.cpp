#include <iostream>
#include <unordered_map>
#include <fstream>
#include <json.hpp>
#include <benchmark_component.h>
#include <unit_test.h>

#include "PricePointBook/OrderBook.h"

using json = nlohmann::json;

int main()
{
    std::cout << "Starting Optimized Price Point Book :)" << std::endl;

    UnitTester unitTester;
    unitTester.RunBasicUnitTest(10000);

    std::ifstream snapshotFile("../Data/snapshot.json");
    json snapshot = json::parse(snapshotFile);
    OrderBook orderBook(snapshot);

    BenchMarkExecution benchMark;
    for(int i = 0; i < 30000; ++i)
    {
        std::ifstream diffUpdateFile("../Data/diffUpdate.json");
        json diffUpdate = json::parse(diffUpdateFile);
        orderBook.ProcessDiffUpdate(diffUpdate);
    }

    benchMark.Print_Time_From_Last_Point();

    return EXIT_SUCCESS;
}
