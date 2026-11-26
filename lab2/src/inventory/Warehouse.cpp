#include "inventory/Warehouse.hpp"

#include "exceptions/InsufficientStockException.hpp"
#include "inventory/SupplyItem.hpp"
#include "inventory/SupplyOrder.hpp"

#include <algorithm>
#include <ctime>
#include <sstream>
#include <stdexcept>
#include <utility>

StockItem& Warehouse::addStockItem(StockItem item) {
    if (StockItem* existing = find(item.name())) {
        existing->replenish(item.quantity());
        return *existing;
    }
    stock_.push_back(std::move(item));
    return stock_.back();
}

bool Warehouse::removeStockItem(const std::string& name) {
    const auto position = std::find_if(
        stock_.begin(), stock_.end(),
        [&name](const StockItem& item) {
            return item.name() == name;
        });
    if (position == stock_.end()) {
        return false;
    }
    stock_.erase(position);
    return true;
}

StockItem* Warehouse::find(const std::string& name) {
    for (auto& item : stock_) {
        if (item.name() == name) {
            return &item;
        }
    }
    return nullptr;
}

const StockItem* Warehouse::find(const std::string& name) const {
    for (const auto& item : stock_) {
        if (item.name() == name) {
            return &item;
        }
    }
    return nullptr;
}

bool Warehouse::hasEnough(const std::string& name, double amount) const {
    const StockItem* item = find(name);
    return item != nullptr && item->available() >= amount;
}

void Warehouse::reserve(const std::string& name, double amount) {
    requireItem(name).reserve(amount);
}

void Warehouse::release(const std::string& name, double amount) {
    requireItem(name).release(amount);
}

void Warehouse::consume(const std::string& name, double amount) {
    requireItem(name).consume(amount);
}

void Warehouse::writeOff(const std::string& name, double amount) {
    requireItem(name).writeOff(amount);
}

std::size_t Warehouse::receiveSupply(const SupplyOrder& order) {
    std::size_t received = 0;
    for (const auto& item : order.items()) {
        if (StockItem* existing = find(item.ingredient().name())) {
            existing->replenish(item.quantity());
        } else {
            stock_.emplace_back(item.ingredient(), item.quantity(), 0.0, 0.0, 0);
        }
        ++received;
    }
    return received;
}

std::vector<const StockItem*> Warehouse::lowStock() const {
    std::vector<const StockItem*> result;
    for (const auto& item : stock_) {
        if (item.isLow()) {
            result.push_back(&item);
        }
    }
    return result;
}

std::vector<const StockItem*> Warehouse::expiringSoon(int days) const {
    std::vector<const StockItem*> result;
    const std::time_t now = std::time(nullptr);
    for (const auto& item : stock_) {
        if (item.isExpiringSoon(now, days)) {
            result.push_back(&item);
        }
    }
    return result;
}

Money Warehouse::totalValue() const {
    Money total;
    for (const auto& item : stock_) {
        total += item.stockValue();
    }
    return total;
}

std::string Warehouse::audit() const {
    std::ostringstream output;
    output << "Warehouse audit (" << stock_.size() << " items, total "
           << totalValue().toString() << ")\n";
    for (const auto& item : stock_) {
        output << "  - " << item.describe() << '\n';
    }
    return output.str();
}

const std::vector<StockItem>& Warehouse::stock() const {
    return stock_;
}

std::size_t Warehouse::itemCount() const {
    return stock_.size();
}

bool Warehouse::empty() const {
    return stock_.empty();
}

StockItem& Warehouse::requireItem(const std::string& name) {
    StockItem* item = find(name);
    if (item == nullptr) {
        throw InsufficientStockException("Unknown stock item: " + name);
    }
    return *item;
}
