#include "services/InventoryService.hpp"

#include "exceptions/InsufficientStockException.hpp"
#include "inventory/SupplyOrder.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

InventoryService::InventoryService() = default;

InventoryService::~InventoryService() = default;

Warehouse& InventoryService::warehouse() {
    return warehouse_;
}

const Warehouse& InventoryService::warehouse() const {
    return warehouse_;
}

StockItem& InventoryService::addStock(StockItem item) {
    return warehouse_.addStockItem(std::move(item));
}

void InventoryService::ensureAvailable(const std::string& name, double amount) const {
    if (!warehouse_.hasEnough(name, amount)) {
        throw InsufficientStockException("Not enough stock available for " + name);
    }
}

void InventoryService::reserve(const std::string& name, double amount) {
    warehouse_.reserve(name, amount);
}

void InventoryService::release(const std::string& name, double amount) {
    warehouse_.release(name, amount);
}

void InventoryService::consume(const std::string& name, double amount) {
    warehouse_.consume(name, amount);
}

void InventoryService::writeOff(const std::string& name, double amount) {
    warehouse_.writeOff(name, amount);
}

void InventoryService::addSupplier(Supplier supplier) {
    if (findSupplier(supplier.name()) != nullptr) {
        throw std::invalid_argument("Duplicate supplier: " + supplier.name());
    }
    suppliers_.push_back(std::move(supplier));
}

Supplier* InventoryService::findSupplier(const std::string& name) {
    for (auto& supplier : suppliers_) {
        if (supplier.name() == name) {
            return &supplier;
        }
    }
    return nullptr;
}

const Supplier* InventoryService::findSupplier(const std::string& name) const {
    for (const auto& supplier : suppliers_) {
        if (supplier.name() == name) {
            return &supplier;
        }
    }
    return nullptr;
}

const std::vector<Supplier>& InventoryService::suppliers() const {
    return suppliers_;
}

SupplyOrder& InventoryService::createSupplyOrder(const std::string& supplierName,
                                                 std::time_t expectedDelivery) {
    const Supplier* supplier = findSupplier(supplierName);
    if (supplier == nullptr) {
        throw std::invalid_argument("Unknown supplier: " + supplierName);
    }
    supplyOrders_.push_back(
        std::make_unique<SupplyOrder>(*supplier, expectedDelivery));
    return *supplyOrders_.back();
}

std::size_t InventoryService::receiveSupply(SupplyOrder& order) {
    if (order.status() == SupplyOrder::Status::Draft) {
        throw std::logic_error("Cannot receive a supply order that was not sent");
    }
    if (order.status() == SupplyOrder::Status::Cancelled) {
        throw std::logic_error("Cannot receive a cancelled supply order");
    }
    if (order.status() != SupplyOrder::Status::Delivered) {
        order.deliver(std::time(nullptr));
    }
    return warehouse_.receiveSupply(order);
}

const std::vector<std::unique_ptr<SupplyOrder>>& InventoryService::supplyOrders() const {
    return supplyOrders_;
}

std::vector<const StockItem*> InventoryService::lowStock() const {
    return warehouse_.lowStock();
}

std::vector<const StockItem*> InventoryService::expiringSoon(int days) const {
    return warehouse_.expiringSoon(days);
}

Money InventoryService::stockValue() const {
    return warehouse_.totalValue();
}
