#include <iostream>
#include <unordered_map>
#include <fstream>

#include "OrderBook.h"

int main()
{
    std::cout << "Starting Optimized Price Point Book :)" << std::endl;
    // key -> Symbol
    // value -> PriceBook(bids, asks)
    std::unordered_map<std::string, PriceBook> symbolBooks;

    std::ifstream initialSnapshot("snapshot.json");
    return EXIT_SUCCESS;
}
