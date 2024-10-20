#pragma once

#include <cmath>
#include <json.hpp>

#include "../PricePointBook/OrderBook.h"

constexpr auto EPSILON = 0.00001;

class UnitTester
{
public:
    void RunBasicUnitTest(int testCount, const std::string& symbol = "BNBBTC")
    {
        OrderBook orderBook;
        for(int i = 0; i < testCount; ++i)
        {
            std::string updateStr =  R"({ "s": ")" + symbol + R"(", "e": "depthUpdate", "U": )" + std::to_string(1000 + i) + R"(, "u": )" + std::to_string(1001 + i) + R"(, "b": [["0.0024", ")" + std::to_string(i) + R"("]], "a": [["0.0026", ")" + std::to_string(i) + R"("]] })";
            nlohmann::json updateJson = nlohmann::json::parse(updateStr);
            orderBook.ProcessDiffUpdate(updateJson);
        }

        if(std::fabs(orderBook.GetPriceBook(symbol)->GetBestAsk() - static_cast<double>(0.0026)) > EPSILON)
        {
            std::cerr << "Best Ask has changed from: " << 0.0026
            << "to: " << orderBook.GetPriceBook(symbol)->GetBestAsk() << std::endl;
        }
        if(std::fabs(orderBook.GetPriceBook(symbol)->GetBestBid() - static_cast<double>(0.0024)) > EPSILON)
        {
            std::cerr << "Best Bid has changed from: " << 0.0024
            << "to: " << orderBook.GetPriceBook(symbol)->GetBestBid() << std::endl;
        }
        if(std::fabs(orderBook.GetPriceBook(symbol)->GetBestAskQuantity() - static_cast<double>(testCount - 1)) > EPSILON)
        {
            std::cerr << "Best Ask has unexpected quantity, expected: " << testCount - 1
            << " actual value: " << orderBook.GetPriceBook(symbol)->GetBestAskQuantity() << std::endl;
        }
        if(std::fabs(orderBook.GetPriceBook(symbol)->GetBestBidQuantity() - static_cast<double>(testCount - 1)) > EPSILON)
        {
            std::cerr << "Best Bid has unexpected quantity, expected: " << testCount - 1
            << " actual value: " << orderBook.GetPriceBook(symbol)->GetBestBidQuantity() << std::endl;
        }
    }
    // ... add more testing tools here
};
