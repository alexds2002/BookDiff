#pragma once

#include "PriceBook.h"

#include <json.hpp>
#include <string>
#include <unordered_map>

class OrderBook
{
public:
    OrderBook() = default;
    ~OrderBook() = default;
    explicit OrderBook(const nlohmann::json& snapshotJson);
    void ProcessSnapshot(const nlohmann::json& snapshotJson);
    void ProcessDiffUpdate(const nlohmann::json& updateJson);
private:
    std::unordered_map<std::string, PriceBook> m_priceBooks;
};
