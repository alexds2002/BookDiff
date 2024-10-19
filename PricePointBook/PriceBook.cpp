#include "PriceBook.h"

#include <iostream>

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

double GetBestAsk() const
{
    if(!m_asks.empty()) { return m_asks.begin()->first; }
    return 0.0;
}

double GetBestBid() const
{
    if(!m_bids.empty()) { return m_bids.begin()->first; }
    return 0.0;
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
