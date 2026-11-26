#include "ordering/KitchenTicket.hpp"

#include "ordering/Order.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
constexpr int kMinimumPriority = 0;
constexpr int kMinimumMinutes = 1;
constexpr double kSecondsPerMinute = 60.0;
}

KitchenTicket::KitchenTicket(Order* order, Chef* chef, int priority, int estimatedMinutes)
    : order_(order),
      chef_(chef),
      priority_(priority),
      estimatedMinutes_(estimatedMinutes),
      status_(Status::Queued),
      createdAt_(std::time(nullptr)),
      startedAt_(0),
      completedAt_(0) {
    if (order_ == nullptr) {
        throw std::invalid_argument("Kitchen ticket order must not be null");
    }
    if (priority_ < kMinimumPriority) {
        throw std::invalid_argument("Kitchen ticket priority must not be negative");
    }
    if (estimatedMinutes_ < kMinimumMinutes) {
        throw std::invalid_argument("Estimated cooking time must be positive");
    }
}

Order* KitchenTicket::order() const {
    return order_;
}

Chef* KitchenTicket::chef() const {
    return chef_;
}

int KitchenTicket::priority() const {
    return priority_;
}

int KitchenTicket::estimatedMinutes() const {
    return estimatedMinutes_;
}

KitchenTicket::Status KitchenTicket::status() const {
    return status_;
}

bool KitchenTicket::isActive() const {
    return status_ == Status::Cooking || status_ == Status::Ready;
}

std::time_t KitchenTicket::createdAt() const {
    return createdAt_;
}

std::time_t KitchenTicket::startedAt() const {
    return startedAt_;
}

std::time_t KitchenTicket::completedAt() const {
    return completedAt_;
}

void KitchenTicket::start() {
    if (status_ != Status::Queued) {
        throw std::logic_error("Only queued tickets can start cooking");
    }
    status_ = Status::Cooking;
    startedAt_ = std::time(nullptr);
}

void KitchenTicket::markReady() {
    if (status_ != Status::Cooking) {
        throw std::logic_error("Only cooking tickets can become ready");
    }
    status_ = Status::Ready;
    completedAt_ = std::time(nullptr);
}

void KitchenTicket::serve() {
    if (status_ != Status::Ready) {
        throw std::logic_error("Only ready tickets can be served");
    }
    status_ = Status::Served;
}

bool KitchenTicket::isOverdue(std::time_t now) const {
    if (status_ != Status::Cooking) {
        return false;
    }
    const double elapsed = std::difftime(now, startedAt_ != 0 ? startedAt_ : createdAt_);
    return elapsed > static_cast<double>(estimatedMinutes_) * kSecondsPerMinute;
}

std::string KitchenTicket::statusLabel() const {
    switch (status_) {
        case Status::Queued:
            return "Queued";
        case Status::Cooking:
            return "Cooking";
        case Status::Ready:
            return "Ready";
        case Status::Served:
            return "Served";
    }
    return "Unknown";
}

std::string KitchenTicket::describe() const {
    std::ostringstream output;
    output << "Ticket for order #" << order_->id() << " [" << statusLabel()
           << "] priority " << priority_ << ", ~" << estimatedMinutes_ << " min";
    return output.str();
}
