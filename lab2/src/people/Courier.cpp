#include "people/Courier.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

Courier::Courier(std::string fullName,
                 std::string phone,
                 std::string email,
                 const Money& monthlySalary,
                 std::time_t hiredAt,
                 Vehicle vehicle,
                 std::string transportNumber)
    : Employee(std::move(fullName), std::move(phone), std::move(email), monthlySalary, hiredAt),
      vehicle_(vehicle),
      transportNumber_(std::move(transportNumber)),
      available_(true),
      completedDeliveries_(0) {}

Courier::Vehicle Courier::vehicle() const {
    return vehicle_;
}

const std::string& Courier::transportNumber() const {
    return transportNumber_;
}

bool Courier::isAvailable() const {
    return available_ && isActive();
}

int Courier::completedDeliveries() const {
    return completedDeliveries_;
}

void Courier::startDelivery() {
    ensureActive();
    if (!available_) {
        throw std::logic_error("Courier " + fullName() + " is already on a delivery");
    }
    available_ = false;
}

void Courier::completeDelivery() {
    if (available_) {
        throw std::logic_error("Courier " + fullName() + " has no active delivery");
    }
    available_ = true;
    ++completedDeliveries_;
}

std::string Courier::vehicleLabel() const {
    switch (vehicle_) {
        case Vehicle::Foot:
            return "On foot";
        case Vehicle::Bicycle:
            return "Bicycle";
        case Vehicle::Scooter:
            return "Scooter";
        case Vehicle::Car:
            return "Car";
    }
    return "Unknown";
}

std::string Courier::role() const {
    return "Courier";
}

std::string Courier::describe() const {
    std::ostringstream output;
    output << Employee::describe() << " | " << vehicleLabel();
    if (!transportNumber_.empty()) {
        output << ' ' << transportNumber_;
    }
    output << " | " << (available_ ? "available" : "on delivery") << " | deliveries "
           << completedDeliveries_;
    return output.str();
}
