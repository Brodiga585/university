#include "people/Waiter.hpp"

#include <sstream>
#include <stdexcept>

Waiter::Waiter(std::string fullName,
               std::string phone,
               std::string email,
               const Money& monthlySalary,
               std::time_t hiredAt,
               int maxTables)
    : Employee(std::move(fullName), std::move(phone), std::move(email), monthlySalary, hiredAt),
      maxTables_(maxTables),
      assignedTables_(0) {
    if (maxTables_ <= 0) {
        throw std::invalid_argument("Waiter max tables must be positive");
    }
}

int Waiter::maxTables() const {
    return maxTables_;
}

int Waiter::assignedTables() const {
    return assignedTables_;
}

bool Waiter::canTakeTable() const {
    return isActive() && assignedTables_ < maxTables_;
}

const Money& Waiter::tips() const {
    return tips_;
}

void Waiter::assignTable() {
    ensureActive();
    if (!canTakeTable()) {
        throw std::logic_error("Waiter " + fullName() + " cannot take another table");
    }
    ++assignedTables_;
}

void Waiter::releaseTable() {
    if (assignedTables_ <= 0) {
        throw std::logic_error("Waiter " + fullName() + " has no assigned tables");
    }
    --assignedTables_;
}

void Waiter::addTip(const Money& tip) {
    if (tip.minorUnits() < 0) {
        throw std::invalid_argument("Tip must not be negative");
    }
    tips_ += tip;
}

std::string Waiter::role() const {
    return "Waiter";
}

std::string Waiter::describe() const {
    std::ostringstream output;
    output << Employee::describe() << " | tables " << assignedTables_ << '/' << maxTables_
           << " | tips " << tips_.toString();
    return output.str();
}
