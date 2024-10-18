#pragma once

#include <map>
#include <string>
#include <json.hpp>

class PriceBook
{
public:
    PriceBook() = default;
    ~PriceBook() = default;
    inline void UpdateBids(double price, double quantity);
    inline void UpdateAsks(double price, double quantity);
    inline void SetLastUpdateID(int id);
    inline int GetLastUpdateID() const;
    inline void ClearPrices();
    void PrintOrderBook() const; // for debug purposes

private:
    std::map<double, double, std::greater<double>> m_bids; // Bid prices in descending order
    std::map<double, double> m_asks;                       // Ask prices in ascending order
    // supports up to 2^31 - 1, overflow should be considered
    int m_lastUpdateId = -1;                               // used to detect inconsistent ID`s

};

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
