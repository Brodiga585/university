#include "people/Chef.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
constexpr int kBaseCapacity = 2;
}

Chef::Chef(std::string fullName,
           std::string phone,
           std::string email,
           const Money& monthlySalary,
           std::time_t hiredAt,
           SkillLevel skillLevel,
           std::string specialty)
    : Employee(std::move(fullName), std::move(phone), std::move(email), monthlySalary, hiredAt),
      skillLevel_(skillLevel),
      specialty_(std::move(specialty)),
      activeTickets_(0) {
    if (specialty_.empty()) {
        throw std::invalid_argument("Chef specialty must not be empty");
    }
}

Chef::SkillLevel Chef::skillLevel() const {
    return skillLevel_;
}

const std::string& Chef::specialty() const {
    return specialty_;
}

int Chef::activeTickets() const {
    return activeTickets_;
}

int Chef::capacity() const {
    return kBaseCapacity + static_cast<int>(skillLevel_);
}

bool Chef::canTakeTicket() const {
    return isActive() && activeTickets_ < capacity();
}

void Chef::beginTicket() {
    ensureActive();
    if (!canTakeTicket()) {
        throw std::logic_error("Chef " + fullName() + " cannot take another ticket");
    }
    ++activeTickets_;
}

void Chef::finishTicket() {
    if (activeTickets_ <= 0) {
        throw std::logic_error("Chef " + fullName() + " has no active tickets");
    }
    --activeTickets_;
}

std::string Chef::role() const {
    return "Chef";
}

std::string Chef::describe() const {
    std::ostringstream output;
    output << Employee::describe() << " | specialty " << specialty_ << " | tickets "
           << activeTickets_ << '/' << capacity();
    return output.str();
}
