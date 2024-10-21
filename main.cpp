#include <iostream>
#include <fstream>
#include <json.hpp>
#include <benchmark_component.h>
#include <unit_test.h>

#include "PricePointBook/OrderBook.h"

// If you want to track the heap memory and memory leaks
// #include <heap_memory_track_component.h>
// TRACK_HEAP_AND_LEAKS();
// PRINT_HEAP_MEMORY();

using json = nlohmann::json;

int main()
{
    std::cout << "Starting Optimized Price Point Book :)" << std::endl;

    // Checks for potential issues
    UnitTester unitTester;
    unitTester.RunBasicUnitTest(10000, "BNBBTC");

    std::ifstream snapshotFile("../Data/snapshot.json");
    json snapshot = json::parse(snapshotFile);
    OrderBook orderBook(snapshot);

    // start tracking execution time
    BenchMarkExecution benchMark;
    for(int i = 0; i < 2000; ++i)
    {
        std::ifstream diffUpdateFile("../Data/diffUpdate.json");
        json diffUpdate = json::parse(diffUpdateFile);
        orderBook.ProcessDiffUpdate(diffUpdate);
    }

    // print execution time
    benchMark.Print_Time_From_Last_Point();

    return EXIT_SUCCESS;
}
