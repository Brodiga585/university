#include "ordering/DeliveryOrder.hpp"

#include "delivery/DeliveryZone.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
constexpr double kDefaultDeliveryFeeRubles = 199.0;
}

DeliveryOrder::DeliveryOrder(std::string customerName,
                             Address address,
                             const DeliveryZone* zone,
                             std::time_t promisedTime)
    : Order(std::move(customerName)),
      address_(std::move(address)),
      zone_(zone),
      courier_(nullptr),
      promisedTime_(promisedTime),
      dispatchedAt_(0),
      deliveredAt_(0) {
    if (promisedTime_ < 0) {
        throw std::invalid_argument("Promised delivery time must not be negative");
    }
}

const Address& DeliveryOrder::address() const {
    return address_;
}

const DeliveryZone* DeliveryOrder::zone() const {
    return zone_;
}

Courier* DeliveryOrder::courier() const {
    return courier_;
}

std::time_t DeliveryOrder::promisedTime() const {
    return promisedTime_;
}

std::time_t DeliveryOrder::dispatchedAt() const {
    return dispatchedAt_;
}

std::time_t DeliveryOrder::deliveredAt() const {
    return deliveredAt_;
}

void DeliveryOrder::setZone(const DeliveryZone* zone) {
    zone_ = zone;
}

void DeliveryOrder::assignCourier(Courier* courier) {
    courier_ = courier;
}

void DeliveryOrder::clearCourier() {
    courier_ = nullptr;
}

void DeliveryOrder::setPromisedTime(std::time_t promisedTime) {
    if (promisedTime < 0) {
        throw std::invalid_argument("Promised delivery time must not be negative");
    }
    promisedTime_ = promisedTime;
}

Money DeliveryOrder::deliveryFee() const {
    if (zone_ == nullptr) {
        return Money::fromRubles(kDefaultDeliveryFeeRubles);
    }
    return zone_->deliveryFee(address_);
}

int DeliveryOrder::estimatedMinutes() const {
    if (zone_ == nullptr) {
        return 0;
    }
    return zone_->estimatedMinutes(address_);
}

bool DeliveryOrder::isLate(std::time_t now) const {
    if (promisedTime_ == 0) {
        return false;
    }
    const std::time_t reference = deliveredAt_ != 0 ? deliveredAt_ : now;
    return std::difftime(reference, promisedTime_) > 0.0;
}

Money DeliveryOrder::extraFee() const {
    return deliveryFee();
}

void DeliveryOrder::dispatch() {
    Order::dispatch();
    dispatchedAt_ = std::time(nullptr);
}

void DeliveryOrder::complete() {
    Order::complete();
    deliveredAt_ = std::time(nullptr);
}

std::string DeliveryOrder::orderType() const {
    return "Delivery";
}

std::string DeliveryOrder::describe() const {
    std::ostringstream output;
    output << Order::describe() << " | to " << address_.format()
           << ", fee " << deliveryFee().toString()
           << ", courier " << (courier_ != nullptr ? "assigned" : "unassigned");
    return output.str();
}
