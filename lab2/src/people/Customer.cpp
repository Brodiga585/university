#include "people/Customer.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

Customer::Customer(std::string fullName,
                   std::string phone,
                   std::string email,
                   std::optional<Address> address)
    : Person(std::move(fullName), std::move(phone), std::move(email)),
      address_(std::move(address)),
      ordersPlaced_(0) {}

const std::optional<Address>& Customer::address() const {
    return address_;
}

int Customer::ordersPlaced() const {
    return ordersPlaced_;
}

const Money& Customer::totalSpent() const {
    return totalSpent_;
}

void Customer::setAddress(Address address) {
    address_ = std::move(address);
}

void Customer::registerOrder() {
    ++ordersPlaced_;
}

void Customer::addSpending(const Money& amount) {
    if (amount.minorUnits() < 0) {
        throw std::invalid_argument("Customer spending must not be negative");
    }
    totalSpent_ += amount;
}

std::string Customer::role() const {
    return "Customer";
}

std::string Customer::describe() const {
    std::ostringstream output;
    output << Person::describe() << " | orders " << ordersPlaced_
           << " | spent " << totalSpent_.toString();
    if (address_.has_value()) {
        output << " | " << address_->format();
    }
    return output.str();
}
