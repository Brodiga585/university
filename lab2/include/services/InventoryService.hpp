#ifndef INVENTORY_SERVICE_HPP
#define INVENTORY_SERVICE_HPP

#include "core/Money.hpp"
#include "inventory/StockItem.hpp"
#include "inventory/Supplier.hpp"
#include "inventory/Warehouse.hpp"

#include <cstddef>
#include <ctime>
#include <memory>
#include <string>
#include <vector>

class SupplyOrder;

class InventoryService {
public:
    InventoryService();
    ~InventoryService();

    Warehouse& warehouse();
    const Warehouse& warehouse() const;

    StockItem& addStock(StockItem item);
    void ensureAvailable(const std::string& name, double amount) const;
    void reserve(const std::string& name, double amount);
    void release(const std::string& name, double amount);
    void consume(const std::string& name, double amount);
    void writeOff(const std::string& name, double amount);

    void addSupplier(Supplier supplier);
    Supplier* findSupplier(const std::string& name);
    const Supplier* findSupplier(const std::string& name) const;
    const std::vector<Supplier>& suppliers() const;

    SupplyOrder& createSupplyOrder(const std::string& supplierName,
                                   std::time_t expectedDelivery);
    std::size_t receiveSupply(SupplyOrder& order);
    const std::vector<std::unique_ptr<SupplyOrder>>& supplyOrders() const;

    std::vector<const StockItem*> lowStock() const;
    std::vector<const StockItem*> expiringSoon(int days) const;
    Money stockValue() const;

private:
    Warehouse warehouse_;
    std::vector<Supplier> suppliers_;
    std::vector<std::unique_ptr<SupplyOrder>> supplyOrders_;
};

#endif
