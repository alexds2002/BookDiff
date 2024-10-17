#include "OrderBook.h"

#include <iostream>

// Can be used to resync
void PriceBook::ProcessSnapshot(const nlohmann::json& snapshotJson)
{
    // Clear existing bids and asks
    m_bids.clear();
    m_asks.clear();

    // Process bids
    for (const auto& bid : snapshotJson["bids"])
    {
        double price = std::stod(bid[0].get<std::string>());
        double quantity = std::stod(bid[1].get<std::string>());
        UpdateBids(price, quantity);
    }

    // Process asks
    for (const auto& ask : snapshotJson["asks"])
    {
        double price = std::stod(ask[0].get<std::string>());
        double quantity = std::stod(ask[1].get<std::string>());
        UpdateAsks(price, quantity);
    }
}

void PriceBook::ProcessDiffUpdate(const nlohmann::json& updateJson)
{
    // Ensure updates are applied sequentially using update IDs
    int firstUpdateId = updateJson["U"];
    int lastUpdateIdInMsg = updateJson["u"];

    if (firstUpdateId <= m_lastUpdateId + 1)
    {
        // Update all bids
        for (const auto& bid : updateJson["b"])
        {
            double price = std::stod(bid[0].get<std::string>());
            double quantity = std::stod(bid[1].get<std::string>());
            UpdateBids(price, quantity);
        }

        // Update all asks
        for (const auto& ask : updateJson["a"])
        {
            double price = std::stod(ask[0].get<std::string>());
            double quantity = std::stod(ask[1].get<std::string>());
            UpdateAsks(price, quantity);
        }

        // Update the last processed update ID
        m_lastUpdateId = lastUpdateIdInMsg;
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
