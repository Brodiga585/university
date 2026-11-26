#include "core/Money.hpp"

#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
constexpr long long kMinorUnitsPerUnit = 100;
constexpr double kMinimumFactor = 0.0;
}

Money::Money() : minorUnits_(0), currency_("RUB") {}

Money::Money(long long minorUnits, std::string currency)
    : minorUnits_(minorUnits), currency_(std::move(currency)) {
    if (currency_.empty()) {
        throw std::invalid_argument("Currency must not be empty");
    }
}

Money Money::fromRubles(double rubles, const std::string& currency) {
    if (!std::isfinite(rubles)) {
        throw std::invalid_argument("Money amount must be finite");
    }
    const long long minorUnits = std::llround(rubles * kMinorUnitsPerUnit);
    return Money(minorUnits, currency);
}

long long Money::minorUnits() const {
    return minorUnits_;
}

double Money::toRubles() const {
    return static_cast<double>(minorUnits_) / kMinorUnitsPerUnit;
}

const std::string& Money::currency() const {
    return currency_;
}

Money Money::operator+(const Money& other) const {
    ensureSameCurrency(other);
    return Money(minorUnits_ + other.minorUnits_, currency_);
}

Money Money::operator-(const Money& other) const {
    ensureSameCurrency(other);
    return Money(minorUnits_ - other.minorUnits_, currency_);
}

Money Money::operator*(double factor) const {
    if (!std::isfinite(factor) || factor < kMinimumFactor) {
        throw std::invalid_argument("Money factor must be finite and non-negative");
    }
    const long long result = std::llround(static_cast<double>(minorUnits_) * factor);
    return Money(result, currency_);
}

Money& Money::operator+=(const Money& other) {
    ensureSameCurrency(other);
    minorUnits_ += other.minorUnits_;
    return *this;
}

bool Money::operator==(const Money& other) const {
    ensureSameCurrency(other);
    return minorUnits_ == other.minorUnits_;
}

bool Money::operator!=(const Money& other) const {
    return !(*this == other);
}

bool Money::operator<(const Money& other) const {
    ensureSameCurrency(other);
    return minorUnits_ < other.minorUnits_;
}

bool Money::operator>(const Money& other) const {
    ensureSameCurrency(other);
    return minorUnits_ > other.minorUnits_;
}

std::string Money::toString() const {
    const long long absolute = std::llabs(minorUnits_);
    std::ostringstream output;
    output << (minorUnits_ < 0 ? "-" : "")
           << absolute / kMinorUnitsPerUnit
           << '.'
           << std::setw(2) << std::setfill('0')
           << absolute % kMinorUnitsPerUnit
           << ' '
           << currency_;
    return output.str();
}

void Money::ensureSameCurrency(const Money& other) const {
    if (currency_ != other.currency_) {
        throw std::invalid_argument("Currency mismatch: " + currency_ + " and " + other.currency_);
    }
}

std::ostream& operator<<(std::ostream& output, const Money& money) {
    return output << money.toString();
}
