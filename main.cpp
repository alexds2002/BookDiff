#include <iostream>
#include <unordered_map>
#include <fstream>

#include "PricePointBook/OrderBook.h"

int main()
{
    std::cout << "Starting Optimized Price Point Book :)" << std::endl;
    std::ifstream initialSnapshot("snapshot.json");
    OrderBook orderBook;
    return EXIT_SUCCESS;
}
