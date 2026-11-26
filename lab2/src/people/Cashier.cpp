#include "people/Cashier.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

Cashier::Cashier(std::string fullName,
                 std::string phone,
                 std::string email,
                 const Money& monthlySalary,
                 std::time_t hiredAt)
    : Employee(std::move(fullName), std::move(phone), std::move(email), monthlySalary, hiredAt),
      processedOrders_(0) {}

int Cashier::processedOrders() const {
    return processedOrders_;
}

const Money& Cashier::collected() const {
    return collected_;
}

void Cashier::registerOrder() {
    ensureActive();
    ++processedOrders_;
}

void Cashier::recordPayment(const Money& amount) {
    ensureActive();
    if (amount.minorUnits() < 0) {
        throw std::invalid_argument("Cashier payment amount must not be negative");
    }
    collected_ += amount;
}

void Cashier::recordRefund(const Money& amount) {
    ensureActive();
    if (amount.minorUnits() < 0) {
        throw std::invalid_argument("Cashier refund amount must not be negative");
    }
    if (collected_ < amount) {
        throw std::invalid_argument("Cashier refund exceeds collected amount");
    }
    collected_ = collected_ - amount;
}

std::string Cashier::role() const {
    return "Cashier";
}

std::string Cashier::describe() const {
    std::ostringstream output;
    output << Employee::describe() << " | orders " << processedOrders_
           << " | collected " << collected_.toString();
    return output.str();
}
