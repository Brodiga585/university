#include "services/DeliveryService.hpp"

#include "exceptions/DeliveryZoneNotCoveredException.hpp"
#include "exceptions/InvalidOrderException.hpp"
#include "ordering/DeliveryOrder.hpp"
#include "people/Courier.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

DeliveryZone& DeliveryService::addZone(DeliveryZone zone) {
    const auto duplicate = std::find_if(
        zones_.begin(), zones_.end(),
        [&zone](const DeliveryZone& current) {
            return current.name() == zone.name();
        });
    if (duplicate != zones_.end()) {
        throw std::invalid_argument("Duplicate delivery zone: " + zone.name());
    }
    zones_.push_back(std::move(zone));
    return zones_.back();
}

const std::vector<DeliveryZone>& DeliveryService::zones() const {
    return zones_;
}

const DeliveryZone* DeliveryService::findZoneFor(const Address& address) const {
    for (const auto& zone : zones_) {
        if (zone.covers(address)) {
            return &zone;
        }
    }
    return nullptr;
}

void DeliveryService::registerCourier(Courier* courier) {
    if (courier == nullptr) {
        throw std::invalid_argument("Courier must not be null");
    }
    if (std::find(couriers_.begin(), couriers_.end(), courier) != couriers_.end()) {
        throw std::invalid_argument("Courier is already registered");
    }
    couriers_.push_back(courier);
}

const std::vector<Courier*>& DeliveryService::couriers() const {
    return couriers_;
}

Courier* DeliveryService::availableCourier() const {
    for (Courier* courier : couriers_) {
        if (courier->isAvailable()) {
            return courier;
        }
    }
    return nullptr;
}

CourierAssignment& DeliveryService::assign(DeliveryOrder& order) {
    Courier* courier = availableCourier();
    if (courier == nullptr) {
        throw std::runtime_error("No available courier");
    }
    return assign(order, *courier);
}

CourierAssignment& DeliveryService::assign(DeliveryOrder& order, Courier& courier) {
    if (order.zone() == nullptr) {
        const DeliveryZone* zone = findZoneFor(order.address());
        if (zone == nullptr) {
            throw DeliveryZoneNotCoveredException(
                "No delivery zone covers " + order.address().format());
        }
        if (order.isPaid()) {
            throw InvalidOrderException(
                "Delivery zone must be assigned before the order is paid");
        }
        order.setZone(zone);
    }
    if (!courier.isAvailable()) {
        throw std::logic_error("Courier is not available");
    }
    if (findAssignment(order.id()) != nullptr) {
        throw std::logic_error("Delivery order already has an assignment");
    }
    CourierAssignment assignment(&courier, &order);
    courier.startDelivery();
    assignments_.push_back(assignment);
    order.assignCourier(&courier);
    return assignments_.back();
}

CourierAssignment* DeliveryService::findAssignment(Entity::Id orderId) {
    for (auto& assignment : assignments_) {
        if (assignment.order()->id() == orderId) {
            return &assignment;
        }
    }
    return nullptr;
}

const CourierAssignment* DeliveryService::findAssignment(Entity::Id orderId) const {
    for (const auto& assignment : assignments_) {
        if (assignment.order()->id() == orderId) {
            return &assignment;
        }
    }
    return nullptr;
}

bool DeliveryService::dispatch(Entity::Id orderId) {
    CourierAssignment* assignment = findAssignment(orderId);
    if (assignment == nullptr) {
        return false;
    }
    DeliveryOrder* order = assignment->order();
    if (order->status() != OrderStatus::Ready) {
        throw InvalidOrderException(
            "Cannot dispatch order in status " + orderStatusLabel(order->status()));
    }
    assignment->confirmPickup();
    order->dispatch();
    return true;
}

bool DeliveryService::complete(Entity::Id orderId) {
    CourierAssignment* assignment = findAssignment(orderId);
    if (assignment == nullptr) {
        return false;
    }
    assignment->complete();
    assignment->order()->complete();
    assignment->courier()->completeDelivery();
    return true;
}

std::size_t DeliveryService::activeAssignments() const {
    std::size_t count = 0;
    for (const auto& assignment : assignments_) {
        if (assignment.isActive()) {
            ++count;
        }
    }
    return count;
}

const std::vector<CourierAssignment>& DeliveryService::assignments() const {
    return assignments_;
}
