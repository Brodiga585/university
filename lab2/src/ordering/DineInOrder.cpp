#include "ordering/DineInOrder.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
constexpr double kMinimumServiceRate = 0.0;
constexpr double kMaximumServiceRate = 1.0;
}

DineInOrder::DineInOrder(std::string customerName,
                         Table* table,
                         int guests,
                         double serviceChargeRate,
                         Waiter* waiter)
    : Order(std::move(customerName)),
      table_(table),
      waiter_(waiter),
      guests_(guests),
      serviceChargeRate_(serviceChargeRate) {
    setGuests(guests_);
    if (!std::isfinite(serviceChargeRate_) || serviceChargeRate_ < kMinimumServiceRate ||
        serviceChargeRate_ > kMaximumServiceRate) {
        throw std::invalid_argument("Service charge rate must be between 0 and 1");
    }
}

Table* DineInOrder::table() const {
    return table_;
}

Waiter* DineInOrder::waiter() const {
    return waiter_;
}

int DineInOrder::guests() const {
    return guests_;
}

double DineInOrder::serviceChargeRate() const {
    return serviceChargeRate_;
}

void DineInOrder::setTable(Table* table) {
    table_ = table;
}

void DineInOrder::setWaiter(Waiter* waiter) {
    waiter_ = waiter;
}

void DineInOrder::setGuests(int guests) {
    if (guests <= 0) {
        throw std::invalid_argument("Guest count must be positive");
    }
    guests_ = guests;
}

Money DineInOrder::extraFee() const {
    return discountedSubtotal() * serviceChargeRate_;
}

std::string DineInOrder::orderType() const {
    return "Dine-in";
}

std::string DineInOrder::describe() const {
    std::ostringstream output;
    output << Order::describe() << " | " << guests_ << " guests"
           << ", table " << (table_ != nullptr ? "assigned" : "unassigned")
           << ", service " << serviceChargeRate_ * 100.0 << '%';
    return output.str();
}
