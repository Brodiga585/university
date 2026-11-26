#include "inventory/Supplier.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
constexpr double kMinimumRating = 0.0;
constexpr double kMaximumRating = 5.0;
}

Supplier::Supplier(std::string name,
                   std::string contactPerson,
                   std::string phone,
                   Address address,
                   std::vector<Weekday> deliveryDays,
                   double rating,
                   const Money& minimumOrder)
    : name_(std::move(name)),
      contactPerson_(std::move(contactPerson)),
      phone_(std::move(phone)),
      address_(std::move(address)),
      deliveryDays_(std::move(deliveryDays)),
      rating_(rating),
      minimumOrder_(minimumOrder) {
    if (name_.empty()) {
        throw std::invalid_argument("Supplier name must not be empty");
    }
    if (contactPerson_.empty()) {
        throw std::invalid_argument("Supplier contact person must not be empty");
    }
    if (phone_.empty()) {
        throw std::invalid_argument("Supplier phone must not be empty");
    }
    if (deliveryDays_.empty()) {
        throw std::invalid_argument("Supplier must have at least one delivery day");
    }
    if (!std::isfinite(rating_) || rating_ < kMinimumRating || rating_ > kMaximumRating) {
        throw std::invalid_argument("Supplier rating must be between 0 and 5");
    }
    if (minimumOrder_.minorUnits() < 0) {
        throw std::invalid_argument("Supplier minimum order must not be negative");
    }
}

const std::string& Supplier::name() const {
    return name_;
}

const std::string& Supplier::contactPerson() const {
    return contactPerson_;
}

const std::string& Supplier::phone() const {
    return phone_;
}

const Address& Supplier::address() const {
    return address_;
}

const std::vector<Supplier::Weekday>& Supplier::deliveryDays() const {
    return deliveryDays_;
}

double Supplier::rating() const {
    return rating_;
}

const Money& Supplier::minimumOrder() const {
    return minimumOrder_;
}

bool Supplier::deliversOn(Weekday weekday) const {
    return std::find(deliveryDays_.begin(), deliveryDays_.end(), weekday) !=
           deliveryDays_.end();
}

bool Supplier::meetsMinimum(const Money& amount) const {
    return !(amount < minimumOrder_);
}

std::string Supplier::weekdayLabel(Weekday weekday) const {
    switch (weekday) {
        case Weekday::Monday:
            return "Monday";
        case Weekday::Tuesday:
            return "Tuesday";
        case Weekday::Wednesday:
            return "Wednesday";
        case Weekday::Thursday:
            return "Thursday";
        case Weekday::Friday:
            return "Friday";
        case Weekday::Saturday:
            return "Saturday";
        case Weekday::Sunday:
            return "Sunday";
    }
    return "";
}

std::string Supplier::describe() const {
    std::ostringstream output;
    output << name_ << " (" << contactPerson_ << ", " << phone_ << ") | "
           << address_.format() << " | rating " << rating_ << " | min order "
           << minimumOrder_.toString() << " | delivers: ";
    for (std::size_t index = 0; index < deliveryDays_.size(); ++index) {
        if (index > 0) {
            output << ", ";
        }
        output << weekdayLabel(deliveryDays_[index]);
    }
    return output.str();
}
