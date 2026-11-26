#include "inventory/StockItem.hpp"

#include "exceptions/InsufficientStockException.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
constexpr double kSecondsPerDay = 86400.0;
constexpr double kMinimumQuantity = 0.0;
}

StockItem::StockItem(Ingredient ingredient,
                     double quantity,
                     double reserved,
                     double minimumQuantity,
                     std::time_t expiryDate)
    : ingredient_(std::move(ingredient)),
      quantity_(quantity),
      reserved_(reserved),
      minimumQuantity_(minimumQuantity),
      expiryDate_(expiryDate) {
    if (!std::isfinite(quantity_) || quantity_ < kMinimumQuantity) {
        throw std::invalid_argument("Stock quantity must be finite and non-negative");
    }
    if (!std::isfinite(reserved_) || reserved_ < kMinimumQuantity) {
        throw std::invalid_argument("Reserved quantity must be finite and non-negative");
    }
    if (reserved_ > quantity_) {
        throw std::invalid_argument("Reserved quantity cannot exceed stock quantity");
    }
    if (!std::isfinite(minimumQuantity_) || minimumQuantity_ < kMinimumQuantity) {
        throw std::invalid_argument("Minimum quantity must be finite and non-negative");
    }
}

const Ingredient& StockItem::ingredient() const {
    return ingredient_;
}

const std::string& StockItem::name() const {
    return ingredient_.name();
}

double StockItem::quantity() const {
    return quantity_;
}

double StockItem::reserved() const {
    return reserved_;
}

double StockItem::available() const {
    return quantity_ - reserved_;
}

double StockItem::minimumQuantity() const {
    return minimumQuantity_;
}

std::time_t StockItem::expiryDate() const {
    return expiryDate_;
}

bool StockItem::isLow() const {
    return available() <= minimumQuantity_;
}

bool StockItem::isExpired(std::time_t now) const {
    return expiryDate_ != 0 && std::difftime(now, expiryDate_) >= 0.0;
}

bool StockItem::isExpiringSoon(std::time_t now, int days) const {
    if (expiryDate_ == 0 || days < 0 || isExpired(now)) {
        return false;
    }
    const double remaining = std::difftime(expiryDate_, now);
    return remaining <= static_cast<double>(days) * kSecondsPerDay;
}

void StockItem::replenish(double amount) {
    ensureAmount(amount, "Replenish");
    quantity_ += amount;
}

void StockItem::reserve(double amount) {
    ensureAmount(amount, "Reserve");
    if (amount > available()) {
        throw InsufficientStockException(
            "Not enough stock to reserve " + name() + ": requested " +
            std::to_string(amount) + ", available " + std::to_string(available()));
    }
    reserved_ += amount;
}

void StockItem::release(double amount) {
    ensureAmount(amount, "Release");
    if (amount > reserved_) {
        throw std::invalid_argument("Cannot release more than reserved for " + name());
    }
    reserved_ -= amount;
}

void StockItem::consume(double amount) {
    ensureAmount(amount, "Consume");
    if (amount > quantity_) {
        throw InsufficientStockException(
            "Not enough stock to consume " + name() + ": requested " +
            std::to_string(amount) + ", on hand " + std::to_string(quantity_));
    }
    quantity_ -= amount;
    if (reserved_ > quantity_) {
        reserved_ = quantity_;
    }
}

void StockItem::writeOff(double amount) {
    ensureAmount(amount, "Write off");
    if (amount > available()) {
        throw InsufficientStockException(
            "Cannot write off more than available for " + name());
    }
    quantity_ -= amount;
}

void StockItem::setMinimumQuantity(double minimumQuantity) {
    if (!std::isfinite(minimumQuantity) || minimumQuantity < kMinimumQuantity) {
        throw std::invalid_argument("Minimum quantity must be finite and non-negative");
    }
    minimumQuantity_ = minimumQuantity;
}

Money StockItem::stockValue() const {
    return ingredient_.costPerUnit() * quantity_;
}

std::string StockItem::describe() const {
    std::ostringstream output;
    output << name() << ": " << quantity_ << ' ' << ingredient_.unitLabel()
           << " on hand (" << reserved_ << " reserved, " << available() << " available)";
    if (isLow()) {
        output << " [low stock]";
    }
    if (expiryDate_ != 0) {
        output << " [expires " << expiryDate_ << ']';
    }
    return output.str();
}

void StockItem::ensureAmount(double amount, const std::string& operation) const {
    if (!std::isfinite(amount) || amount <= kMinimumQuantity) {
        throw std::invalid_argument(operation + " amount must be finite and positive");
    }
}
