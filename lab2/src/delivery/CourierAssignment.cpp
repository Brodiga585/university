#include "delivery/CourierAssignment.hpp"

#include "ordering/DeliveryOrder.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>

namespace {
constexpr double kSecondsPerMinute = 60.0;
}

CourierAssignment::CourierAssignment(Courier* courier, DeliveryOrder* order)
    : courier_(courier),
      order_(order),
      status_(Status::Assigned),
      assignedAt_(std::time(nullptr)),
      pickedUpAt_(0),
      deliveredAt_(0) {
    if (courier_ == nullptr) {
        throw std::invalid_argument("Courier must not be null");
    }
    if (order_ == nullptr) {
        throw std::invalid_argument("Delivery order must not be null");
    }
}

Courier* CourierAssignment::courier() const {
    return courier_;
}

DeliveryOrder* CourierAssignment::order() const {
    return order_;
}

CourierAssignment::Status CourierAssignment::status() const {
    return status_;
}

bool CourierAssignment::isActive() const {
    return status_ == Status::Assigned || status_ == Status::OnTheWay;
}

std::time_t CourierAssignment::assignedAt() const {
    return assignedAt_;
}

std::time_t CourierAssignment::pickedUpAt() const {
    return pickedUpAt_;
}

std::time_t CourierAssignment::deliveredAt() const {
    return deliveredAt_;
}

void CourierAssignment::confirmPickup() {
    if (status_ != Status::Assigned) {
        throw std::logic_error("Only an assigned courier can pick up an order");
    }
    status_ = Status::OnTheWay;
    pickedUpAt_ = std::time(nullptr);
}

void CourierAssignment::complete() {
    if (status_ != Status::OnTheWay) {
        throw std::logic_error("Only an order on the way can be delivered");
    }
    status_ = Status::Delivered;
    deliveredAt_ = std::time(nullptr);
}

void CourierAssignment::cancel() {
    if (!isActive()) {
        throw std::logic_error("Cannot cancel a finished courier assignment");
    }
    status_ = Status::Cancelled;
}

int CourierAssignment::durationMinutes(std::time_t now) const {
    if (pickedUpAt_ == 0) {
        return 0;
    }
    const std::time_t finish = deliveredAt_ != 0 ? deliveredAt_ : now;
    const double seconds = std::difftime(finish, pickedUpAt_);
    return static_cast<int>(std::lround(seconds / kSecondsPerMinute));
}

bool CourierAssignment::isLate(std::time_t now) const {
    return status_ != Status::Cancelled && order_->isLate(now);
}

std::string CourierAssignment::statusLabel() const {
    switch (status_) {
        case Status::Assigned:
            return "Assigned";
        case Status::OnTheWay:
            return "On the way";
        case Status::Delivered:
            return "Delivered";
        case Status::Cancelled:
            return "Cancelled";
    }
    return "Unknown";
}

std::string CourierAssignment::describe() const {
    std::ostringstream output;
    output << "Courier assignment for order #" << order_->id() << " ["
           << statusLabel() << ']';
    return output.str();
}
