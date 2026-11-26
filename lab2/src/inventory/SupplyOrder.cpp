#include "inventory/SupplyOrder.hpp"

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <utility>

SupplyOrder::SupplyOrder(Supplier supplier, std::time_t expectedDelivery)
    : supplier_(std::move(supplier)),
      status_(Status::Draft),
      expectedDelivery_(expectedDelivery),
      deliveredAt_(0) {
    if (expectedDelivery_ <= 0) {
        throw std::invalid_argument("Supply order expected delivery must be positive");
    }
}

const Supplier& SupplyOrder::supplier() const {
    return supplier_;
}

const std::vector<SupplyItem>& SupplyOrder::items() const {
    return items_;
}

SupplyOrder::Status SupplyOrder::status() const {
    return status_;
}

std::time_t SupplyOrder::expectedDelivery() const {
    return expectedDelivery_;
}

std::time_t SupplyOrder::deliveredAt() const {
    return deliveredAt_;
}

void SupplyOrder::addItem(SupplyItem item) {
    ensureStatus(Status::Draft, "add items to");
    const auto duplicate = std::find_if(
        items_.begin(), items_.end(),
        [&item](const SupplyItem& current) {
            return current.name() == item.name();
        });
    if (duplicate != items_.end()) {
        throw std::invalid_argument("Duplicate supply item: " + item.name());
    }
    items_.push_back(std::move(item));
}

bool SupplyOrder::removeItem(const std::string& name) {
    ensureStatus(Status::Draft, "remove items from");
    const auto position = std::find_if(
        items_.begin(), items_.end(),
        [&name](const SupplyItem& item) {
            return item.name() == name;
        });
    if (position == items_.end()) {
        return false;
    }
    items_.erase(position);
    return true;
}

Money SupplyOrder::total() const {
    Money result;
    for (const auto& item : items_) {
        result += item.total();
    }
    return result;
}

std::size_t SupplyOrder::itemCount() const {
    return items_.size();
}

void SupplyOrder::send() {
    ensureStatus(Status::Draft, "send");
    if (items_.empty()) {
        throw std::logic_error("Cannot send an empty supply order");
    }
    if (!supplier_.meetsMinimum(total())) {
        throw std::logic_error(
            "Supply order total is below the supplier minimum of " +
            supplier_.minimumOrder().toString());
    }
    status_ = Status::Sent;
}

void SupplyOrder::confirm() {
    ensureStatus(Status::Sent, "confirm");
    status_ = Status::Confirmed;
}

void SupplyOrder::deliver(std::time_t deliveredAt) {
    if (status_ != Status::Sent && status_ != Status::Confirmed) {
        throw std::logic_error("Only sent or confirmed supply orders can be delivered");
    }
    if (deliveredAt <= 0) {
        throw std::invalid_argument("Delivery timestamp must be positive");
    }
    status_ = Status::Delivered;
    deliveredAt_ = deliveredAt;
}

void SupplyOrder::cancel() {
    if (status_ == Status::Delivered || status_ == Status::Cancelled) {
        throw std::logic_error("Cannot cancel a completed supply order");
    }
    status_ = Status::Cancelled;
}

bool SupplyOrder::isLate(std::time_t now) const {
    return isOpen() && std::difftime(now, expectedDelivery_) > 0.0;
}

bool SupplyOrder::isOpen() const {
    return status_ != Status::Delivered && status_ != Status::Cancelled;
}

std::string SupplyOrder::statusLabel() const {
    switch (status_) {
        case Status::Draft:
            return "Draft";
        case Status::Sent:
            return "Sent";
        case Status::Confirmed:
            return "Confirmed";
        case Status::Delivered:
            return "Delivered";
        case Status::Cancelled:
            return "Cancelled";
    }
    return "Unknown";
}

std::string SupplyOrder::describe() const {
    std::ostringstream output;
    output << "Supply order #" << id() << " from " << supplier_.name()
           << " [" << statusLabel() << "] " << itemCount() << " items, total "
           << total().toString();
    return output.str();
}

void SupplyOrder::ensureStatus(Status expected, const std::string& operation) const {
    if (status_ != expected) {
        throw std::logic_error("Cannot " + operation + " supply order in status " +
                               statusLabel());
    }
}
