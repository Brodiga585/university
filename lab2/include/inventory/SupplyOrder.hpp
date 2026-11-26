#ifndef SUPPLY_ORDER_HPP
#define SUPPLY_ORDER_HPP

#include "core/Entity.hpp"
#include "core/Money.hpp"
#include "inventory/Supplier.hpp"
#include "inventory/SupplyItem.hpp"

#include <cstddef>
#include <ctime>
#include <string>
#include <vector>

class SupplyOrder : public Entity {
public:
    enum class Status {
        Draft,
        Sent,
        Confirmed,
        Delivered,
        Cancelled
    };

    SupplyOrder(Supplier supplier, std::time_t expectedDelivery);

    const Supplier& supplier() const;
    const std::vector<SupplyItem>& items() const;
    Status status() const;
    std::time_t expectedDelivery() const;
    std::time_t deliveredAt() const;

    void addItem(SupplyItem item);
    bool removeItem(const std::string& name);
    Money total() const;
    std::size_t itemCount() const;

    void send();
    void confirm();
    void deliver(std::time_t deliveredAt);
    void cancel();

    bool isLate(std::time_t now) const;
    bool isOpen() const;
    std::string statusLabel() const;
    std::string describe() const;

private:
    void ensureStatus(Status expected, const std::string& operation) const;

    Supplier supplier_;
    std::vector<SupplyItem> items_;
    Status status_;
    std::time_t expectedDelivery_;
    std::time_t deliveredAt_;
};

#endif
