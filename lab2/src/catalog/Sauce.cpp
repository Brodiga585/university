#include "catalog/Sauce.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

namespace {
constexpr int kMinimumSpiciness = 0;
constexpr int kMaximumSpiciness = 5;
}

Sauce::Sauce(std::string name, int spiciness, double priceFactor, bool vegetarian)
    : name_(std::move(name)),
      spiciness_(spiciness),
      priceFactor_(priceFactor),
      vegetarian_(vegetarian) {
    if (name_.empty()) {
        throw std::invalid_argument("Sauce name must not be empty");
    }
    if (spiciness_ < kMinimumSpiciness || spiciness_ > kMaximumSpiciness) {
        throw std::invalid_argument("Sauce spiciness must be between 0 and 5");
    }
    if (!std::isfinite(priceFactor_) || priceFactor_ < 0.0) {
        throw std::invalid_argument("Sauce price factor must be finite and non-negative");
    }
}

const std::string& Sauce::name() const {
    return name_;
}

int Sauce::spiciness() const {
    return spiciness_;
}

double Sauce::priceFactor() const {
    return priceFactor_;
}

bool Sauce::isVegetarian() const {
    return vegetarian_;
}

Money Sauce::surcharge(const Money& basePrice) const {
    const double extraFactor = std::max(0.0, priceFactor_ - 1.0);
    return basePrice * extraFactor;
}

bool Sauce::isSpicy() const {
    return spiciness_ > kMinimumSpiciness;
}

std::string Sauce::label() const {
    return name_;
}
