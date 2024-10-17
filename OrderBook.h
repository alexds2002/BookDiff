#pragma once

#include <map>
#include <string>
#include <json.hpp>

class PriceBook
{
public:
    void ProcessSnapshot(const nlohmann::json& snapshotJson);
    void ProcessDiffUpdate(const nlohmann::json& updateJson);
    inline void UpdateBids(double price, double quantity);
    inline void UpdateAsks(double price, double quantity);
    void PrintOrderBook() const; // for debug purposes

private:
    std::map<double, double, std::greater<double>> m_bids; // Bid prices in descending order
    std::map<double, double> m_asks;                       // Ask prices in ascending order
    int m_lastUpdateId = -1; // used to detect inconsistent ID`s

};
