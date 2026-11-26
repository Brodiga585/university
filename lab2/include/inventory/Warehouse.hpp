#ifndef WAREHOUSE_HPP
#define WAREHOUSE_HPP

#include "inventory/StockItem.hpp"

#include <cstddef>
#include <string>
#include <vector>

class SupplyOrder;

class Warehouse {
public:
    Warehouse() = default;

    StockItem& addStockItem(StockItem item);
    bool removeStockItem(const std::string& name);

    StockItem* find(const std::string& name);
    const StockItem* find(const std::string& name) const;
    bool hasEnough(const std::string& name, double amount) const;

    void reserve(const std::string& name, double amount);
    void release(const std::string& name, double amount);
    void consume(const std::string& name, double amount);
    void writeOff(const std::string& name, double amount);
    std::size_t receiveSupply(const SupplyOrder& order);

    std::vector<const StockItem*> lowStock() const;
    std::vector<const StockItem*> expiringSoon(int days) const;
    Money totalValue() const;
    std::string audit() const;

    const std::vector<StockItem>& stock() const;
    std::size_t itemCount() const;
    bool empty() const;

private:
    StockItem& requireItem(const std::string& name);

    std::vector<StockItem> stock_;
};

#endif
