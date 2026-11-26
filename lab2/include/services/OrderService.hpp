#ifndef ORDER_SERVICE_HPP
#define ORDER_SERVICE_HPP

#include "core/Address.hpp"
#include "core/Entity.hpp"
#include "core/Money.hpp"
#include "ordering/Order.hpp"
#include "ordering/OrderStatus.hpp"

#include <cstddef>
#include <ctime>
#include <memory>
#include <string>
#include <vector>

class DeliveryOrder;
class DeliveryZone;
class DineInOrder;
class Order;
class Table;
class TakeawayOrder;

class OrderService {
public:
    OrderService() = default;

    DineInOrder& createDineIn(std::string customerName,
                             Table* table,
                             int guests,
                             double serviceChargeRate = 0.1);
    DeliveryOrder& createDelivery(std::string customerName,
                                  Address address,
                                  const DeliveryZone* zone = nullptr,
                                  std::time_t promisedTime = 0);
    TakeawayOrder& createTakeaway(std::string customerName,
                                  std::string pickupCode = {},
                                  std::time_t readyAt = 0);

    Order* find(Entity::Id id);
    const Order* find(Entity::Id id) const;
    Order& require(Entity::Id id);
    const Order& require(Entity::Id id) const;
    bool remove(Entity::Id id);

    const std::vector<std::unique_ptr<Order>>& orders() const;
    std::size_t count() const;
    std::vector<const Order*> byStatus(OrderStatus status) const;
    std::vector<const Order*> activeOrders() const;
    Money completedRevenue() const;

private:
    std::vector<std::unique_ptr<Order>> orders_;
};

#endif
