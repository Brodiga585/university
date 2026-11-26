#include "venue/Shift.hpp"

#include "people/Employee.hpp"

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
constexpr double kSecondsPerMinute = 60.0;
constexpr double kMonthlyWorkingMinutes = 160.0 * 60.0;
}

Shift::Shift(std::string title, std::time_t startsAt, int durationMinutes)
    : title_(std::move(title)), startsAt_(startsAt), durationMinutes_(durationMinutes), open_(true) {
    if (title_.empty()) {
        throw std::invalid_argument("Shift title must not be empty");
    }
    if (startsAt_ <= 0) {
        throw std::invalid_argument("Shift start time must be positive");
    }
    if (durationMinutes_ <= 0) {
        throw std::invalid_argument("Shift duration must be positive");
    }
}

const std::string& Shift::title() const {
    return title_;
}

std::time_t Shift::startsAt() const {
    return startsAt_;
}

std::time_t Shift::endsAt() const {
    return startsAt_ +
           static_cast<std::time_t>(durationMinutes_ * kSecondsPerMinute);
}

int Shift::durationMinutes() const {
    return durationMinutes_;
}

bool Shift::isOpen() const {
    return open_;
}

void Shift::addEmployee(Employee* employee) {
    if (!open_) {
        throw std::logic_error("Cannot add employees to a closed shift");
    }
    if (employee == nullptr) {
        throw std::invalid_argument("Shift employee must not be null");
    }
    if (std::find(employees_.begin(), employees_.end(), employee) != employees_.end()) {
        throw std::invalid_argument("Employee is already assigned to this shift");
    }
    employees_.push_back(employee);
}

bool Shift::removeEmployee(Employee* employee) {
    const auto position = std::find(employees_.begin(), employees_.end(), employee);
    if (position == employees_.end()) {
        return false;
    }
    employees_.erase(position);
    return true;
}

const std::vector<Employee*>& Shift::employees() const {
    return employees_;
}

std::size_t Shift::employeeCount() const {
    return employees_.size();
}

bool Shift::covers(std::time_t moment) const {
    return open_ && moment >= startsAt_ && moment < endsAt();
}

Money Shift::payrollCost() const {
    Money total;
    const double share = (durationMinutes_ * kSecondsPerMinute) / kMonthlyWorkingMinutes;
    for (const Employee* employee : employees_) {
        total += employee->monthlySalary() * share;
    }
    return total;
}

void Shift::close() {
    if (!open_) {
        throw std::logic_error("Shift is already closed");
    }
    open_ = false;
}

std::string Shift::describe() const {
    std::ostringstream output;
    output << "Shift '" << title_ << "' from " << startsAt_ << " to " << endsAt()
           << " [" << (open_ ? "open" : "closed") << "], " << employees_.size()
           << " employees, payroll " << payrollCost().toString();
    return output.str();
}
