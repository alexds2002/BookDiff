#include <iostream>
#include <fstream>
#include <json.hpp>
#include <benchmark_component.h>
#include <unit_test.h>

#include "PricePointBook/OrderBook.h"

/*
 * Optimization ideas latency:
 * Add a ThreadPool(if handling events in parallel is possible and there are no constrains: for example checking each event ID agains the previous)
 * Optimize the json parsing. If the json lib bottlenecks either change the library or implement a custom one for the specific needs.
 * Collect a group of updates before handling them. This can reduce number of lookups and operations on the data structure.
 * Lazy Updates. Update values only when the best price changes.
 *
 * Optimization ideas memory:
 * Use simpler data structures, like a prealocated array
 * Use compact(aligned) structs to store bids/asks
 * Store bids/asks in integers instead of float(bit gymnastics overhead)
 */

// Uncomment to track heap memory.
// #include <heap_memory_track_component.h>
// TRACK_HEAP_AND_LEAKS();
// Use the macro PRINT_HEAP_MEMORY(); to check memory at any point

using json = nlohmann::json;

constexpr int NUMBER_OF_UPDATES = 1000000;

int main()
{
    std::cout << "Starting Optimized Price Point Book... :)" << std::endl;
    std::cout << "To track memory use the heap tracker component. See main.cpp." << std::endl;

    // Checks for potential issues
    UnitTester unitTester;
    unitTester.RunBasicUnitTest(10000, "BNBBTC");

    std::ifstream snapshotFile("../Data/snapshot.json");
    json snapshot = json::parse(snapshotFile);
    OrderBook orderBook(snapshot);

    std::cout << "Load and Parse: " << NUMBER_OF_UPDATES << " updates." << std::endl;
    BenchMarkExecution benchMark;
    for(int i = 0; i < NUMBER_OF_UPDATES; ++i)
    {
        std::ifstream diffUpdateFile("../Data/diffUpdate.json");
        json diffUpdate = json::parse(diffUpdateFile);
        orderBook.ProcessDiffUpdate(diffUpdate);
    }

    // print execution time
    benchMark.Print_Time_From_Last_Point();
    // print memory
    // PRINT_HEAP_MEMORY();

    return EXIT_SUCCESS;
}
