#include "catalog/Dough.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

Dough::Dough(Type type, std::string description, double priceFactor)
    : type_(type), description_(std::move(description)), priceFactor_(priceFactor) {
    if (description_.empty()) {
        throw std::invalid_argument("Dough description must not be empty");
    }
    if (!std::isfinite(priceFactor_) || priceFactor_ < 0.0) {
        throw std::invalid_argument("Dough price factor must be finite and non-negative");
    }
}

Dough::Type Dough::type() const {
    return type_;
}

const std::string& Dough::description() const {
    return description_;
}

double Dough::priceFactor() const {
    return priceFactor_;
}

Money Dough::surcharge(const Money& basePrice) const {
    const double extraFactor = std::max(0.0, priceFactor_ - 1.0);
    return basePrice * extraFactor;
}

bool Dough::isGlutenFree() const {
    return type_ == Type::GlutenFree;
}

std::string Dough::label() const {
    return description_;
}
