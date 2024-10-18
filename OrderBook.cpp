#include "OrderBook.h"

#include <iostream>

// asuming that the snapshot contains the Symbol, else we have to get it from somewhere else
OrderBook::OrderBook(const nlohmann::json& snapshotJson)
{
    ProcessSnapshot(snapshotJson);
}

// Can be used to resync
void OrderBook::ProcessSnapshot(const nlohmann::json& snapshotJson)
{
    std::string msgSymbol = snapshotJson["s"];
    PriceBook& curPriceBook = m_priceBooks[msgSymbol];
    curPriceBook.ClearPrices();
    // Process bids
    for (const auto& bid : snapshotJson["bids"])
    {
        double price = std::stod(bid[0].get<std::string>());
        double quantity = std::stod(bid[1].get<std::string>());
        curPriceBook.UpdateBids(price, quantity);
    }

    // Process asks
    for (const auto& ask : snapshotJson["asks"])
    {
        double price = std::stod(ask[0].get<std::string>());
        double quantity = std::stod(ask[1].get<std::string>());
        curPriceBook.UpdateAsks(price, quantity);
    }
}

void OrderBook::ProcessDiffUpdate(const nlohmann::json& updateJson)
{
    std::string msgSymbol = updateJson["s"];
    PriceBook& curPriceBook = m_priceBooks[msgSymbol];

    // Ensure updates are applied sequentially using update IDs
    int firstUpdateId = updateJson["U"];
    int lastUpdateIdInMsg = updateJson["u"];

    if (firstUpdateId <= curPriceBook.GetLastUpdateID() + 1)
    {
        // Update all bids
        for (const auto& bid : updateJson["b"])
        {
            double price = std::stod(bid[0].get<std::string>());
            double quantity = std::stod(bid[1].get<std::string>());
            curPriceBook.UpdateBids(price, quantity);
        }

        // Update all asks
        for (const auto& ask : updateJson["a"])
        {
            double price = std::stod(ask[0].get<std::string>());
            double quantity = std::stod(ask[1].get<std::string>());
            curPriceBook.UpdateAsks(price, quantity);
        }

        // Update the last processed update ID
        curPriceBook.SetLastUpdateID(lastUpdateIdInMsg);
    }
    else
    {
        std::cerr << "Missed updates, resynchronization required!" << std::endl;
        // call ProccessSnapshot or exit the program
    }
}

void PriceBook::UpdateBids(double price, double quantity)
{
    if (quantity == 0)
    {
        m_bids.erase(price); // Remove bid if quantity is 0
    }
    else
    {
        m_bids[price] = quantity; // Update or insert the bid
    }
}

void PriceBook::UpdateAsks(double price, double quantity)
{
    if (quantity == 0)
    {
        m_asks.erase(price); // Remove ask if quantity is 0
    }
    else
    {
        m_asks[price] = quantity; // Update or insert the ask
    }
}

void PriceBook::SetLastUpdateID(int id)
{
    m_lastUpdateId = id;
}

int PriceBook::GetLastUpdateID() const
{
    return m_lastUpdateId;
}

// Clear existing bids and asks
void PriceBook::ClearPrices()
{
    m_bids.clear();
    m_asks.clear();
}

// for debug purposes
void PriceBook::PrintOrderBook() const
{
    std::cout << "Order Book - Bids:" << std::endl;
    for (const auto& [price, quantity] : m_bids)
    {
        std::cout << price << " : " << quantity << "\n";
    }

    std::cout << "Order Book - Asks:" << std::endl;
    for (const auto& [price, quantity] : m_asks)
    {
        std::cout << price << " : " << quantity << "\n";
    }
}
