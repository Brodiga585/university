#include "people/Employee.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
constexpr double kSecondsPerYear = 365.25 * 86400.0;
}

Employee::Employee(std::string fullName,
                   std::string phone,
                   std::string email,
                   const Money& monthlySalary,
                   std::time_t hiredAt,
                   Status status)
    : Person(std::move(fullName), std::move(phone), std::move(email)),
      monthlySalary_(monthlySalary),
      hiredAt_(hiredAt),
      status_(status) {
    if (monthlySalary_.minorUnits() < 0) {
        throw std::invalid_argument("Employee salary must not be negative");
    }
    if (hiredAt_ <= 0) {
        throw std::invalid_argument("Employee hire date must be positive");
    }
}

const Money& Employee::monthlySalary() const {
    return monthlySalary_;
}

std::time_t Employee::hiredAt() const {
    return hiredAt_;
}

Employee::Status Employee::status() const {
    return status_;
}

bool Employee::isActive() const {
    return status_ == Status::Active;
}

void Employee::setMonthlySalary(const Money& salary) {
    if (salary.minorUnits() < 0) {
        throw std::invalid_argument("Employee salary must not be negative");
    }
    monthlySalary_ = salary;
}

void Employee::setStatus(Status status) {
    status_ = status;
}

int Employee::yearsOfService(std::time_t now) const {
    if (now <= hiredAt_) {
        return 0;
    }
    const double years = std::difftime(now, hiredAt_) / kSecondsPerYear;
    return static_cast<int>(std::floor(years));
}

std::string Employee::statusLabel() const {
    switch (status_) {
        case Status::Active:
            return "Active";
        case Status::OnLeave:
            return "On leave";
        case Status::Dismissed:
            return "Dismissed";
    }
    return "Unknown";
}

std::string Employee::describe() const {
    std::ostringstream output;
    output << Person::describe() << " | salary " << monthlySalary_.toString()
           << "/month | " << statusLabel();
    return output.str();
}

void Employee::ensureActive() const {
    if (!isActive()) {
        throw std::logic_error("Employee is not active: " + fullName());
    }
}
