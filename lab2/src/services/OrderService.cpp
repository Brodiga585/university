#include "services/OrderService.hpp"

#include "exceptions/OrderNotFoundException.hpp"
#include "ordering/DeliveryOrder.hpp"
#include "ordering/DineInOrder.hpp"
#include "ordering/Order.hpp"
#include "ordering/TakeawayOrder.hpp"

#include <algorithm>
#include <sstream>
#include <utility>

DineInOrder& OrderService::createDineIn(std::string customerName,
                                        Table* table,
                                        int guests,
                                        double serviceChargeRate) {
    orders_.push_back(std::make_unique<DineInOrder>(
        std::move(customerName), table, guests, serviceChargeRate));
    return *static_cast<DineInOrder*>(orders_.back().get());
}

DeliveryOrder& OrderService::createDelivery(std::string customerName,
                                            Address address,
                                            const DeliveryZone* zone,
                                            std::time_t promisedTime) {
    orders_.push_back(std::make_unique<DeliveryOrder>(
        std::move(customerName), std::move(address), zone, promisedTime));
    return *static_cast<DeliveryOrder*>(orders_.back().get());
}

TakeawayOrder& OrderService::createTakeaway(std::string customerName,
                                            std::string pickupCode,
                                            std::time_t readyAt) {
    orders_.push_back(std::make_unique<TakeawayOrder>(
        std::move(customerName), std::move(pickupCode), readyAt));
    return *static_cast<TakeawayOrder*>(orders_.back().get());
}

Order* OrderService::find(Entity::Id id) {
    for (auto& order : orders_) {
        if (order->id() == id) {
            return order.get();
        }
    }
    return nullptr;
}

const Order* OrderService::find(Entity::Id id) const {
    for (const auto& order : orders_) {
        if (order->id() == id) {
            return order.get();
        }
    }
    return nullptr;
}

Order& OrderService::require(Entity::Id id) {
    Order* order = find(id);
    if (order == nullptr) {
        throw OrderNotFoundException("Order not found: #" + std::to_string(id));
    }
    return *order;
}

const Order& OrderService::require(Entity::Id id) const {
    const Order* order = find(id);
    if (order == nullptr) {
        throw OrderNotFoundException("Order not found: #" + std::to_string(id));
    }
    return *order;
}

bool OrderService::remove(Entity::Id id) {
    const auto position = std::find_if(
        orders_.begin(), orders_.end(),
        [id](const std::unique_ptr<Order>& order) {
            return order->id() == id;
        });
    if (position == orders_.end()) {
        return false;
    }
    orders_.erase(position);
    return true;
}

const std::vector<std::unique_ptr<Order>>& OrderService::orders() const {
    return orders_;
}

std::size_t OrderService::count() const {
    return orders_.size();
}

std::vector<const Order*> OrderService::byStatus(OrderStatus status) const {
    std::vector<const Order*> result;
    for (const auto& order : orders_) {
        if (order->status() == status) {
            result.push_back(order.get());
        }
    }
    return result;
}

std::vector<const Order*> OrderService::activeOrders() const {
    std::vector<const Order*> result;
    for (const auto& order : orders_) {
        if (isActiveStatus(order->status())) {
            result.push_back(order.get());
        }
    }
    return result;
}

Money OrderService::completedRevenue() const {
    Money total;
    for (const auto& order : orders_) {
        if (order->status() == OrderStatus::Completed) {
            total += order->total();
        }
    }
    return total;
}
