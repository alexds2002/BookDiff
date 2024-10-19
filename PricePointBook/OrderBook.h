#pragma once

#include "PriceBook.h"

#include <json.hpp>
#include <unordered_map>
#include <string>

class OrderBook
{
public:
    OrderBook() = default;
    ~OrderBook() = default;
    explicit OrderBook(const nlohmann::json& snapshotJson);
    void ProcessSnapshot(const nlohmann::json& snapshotJson);
    void ProcessDiffUpdate(const nlohmann::json& updateJson);
    PriceBook* GetPriceBook(const std::string& symbol);
private:
    std::unordered_map<std::string, PriceBook> m_priceBooks;
};
