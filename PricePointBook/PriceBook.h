#pragma once

#include <map>

class PriceBook
{
public:
    PriceBook() = default;
    ~PriceBook() = default;
    void UpdateBids(double price, double quantity);
    void UpdateAsks(double price, double quantity);
    void SetLastUpdateID(int id);
    int GetLastUpdateID() const;
    void ClearPrices();
    void PrintOrderBook() const; // for debug purposes

private:
    std::map<double, double, std::greater<double>> m_bids; // Bid prices in descending order
    std::map<double, double> m_asks;                       // Ask prices in ascending order
    // supports up to 2^31 - 1, overflow should be considered
    int m_lastUpdateId = -1;                               // used to detect inconsistent ID`s

};

