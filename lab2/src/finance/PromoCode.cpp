#include "finance/PromoCode.hpp"

#include "exceptions/InvalidPromoCodeException.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
constexpr double kMinimumPercent = 0.0;
constexpr double kMaximumPercent = 100.0;
constexpr int kUnlimitedUses = 0;
}

PromoCode::PromoCode(std::string code,
                     double discountPercent,
                     const Money& minimumOrder,
                     std::time_t validFrom,
                     std::time_t validUntil,
                     int maxUses)
    : code_(std::move(code)),
      discountPercent_(discountPercent),
      minimumOrder_(minimumOrder),
      validFrom_(validFrom),
      validUntil_(validUntil),
      maxUses_(maxUses),
      usedCount_(0),
      active_(true) {
    if (code_.empty()) {
        throw std::invalid_argument("Promo code must not be empty");
    }
    if (!std::isfinite(discountPercent_) || discountPercent_ <= kMinimumPercent ||
        discountPercent_ > kMaximumPercent) {
        throw std::invalid_argument("Promo discount must be between 0 and 100 percent");
    }
    if (minimumOrder_.minorUnits() < 0) {
        throw std::invalid_argument("Promo minimum order must not be negative");
    }
    if (validFrom_ <= 0 || validUntil_ <= validFrom_) {
        throw std::invalid_argument("Promo validity interval is invalid");
    }
    if (maxUses_ < kUnlimitedUses) {
        throw std::invalid_argument("Promo max uses must not be negative");
    }
}

const std::string& PromoCode::code() const {
    return code_;
}

double PromoCode::discountPercent() const {
    return discountPercent_;
}

const Money& PromoCode::minimumOrder() const {
    return minimumOrder_;
}

std::time_t PromoCode::validFrom() const {
    return validFrom_;
}

std::time_t PromoCode::validUntil() const {
    return validUntil_;
}

int PromoCode::maxUses() const {
    return maxUses_;
}

int PromoCode::usedCount() const {
    return usedCount_;
}

int PromoCode::remainingUses() const {
    if (maxUses_ == kUnlimitedUses) {
        return -1;
    }
    return maxUses_ - usedCount_;
}

bool PromoCode::isActive() const {
    return active_;
}

void PromoCode::setActive(bool active) {
    active_ = active;
}

bool PromoCode::isValidFor(const Money& orderTotal, std::time_t now) const {
    if (!active_) {
        return false;
    }
    if (now < validFrom_ || now > validUntil_) {
        return false;
    }
    if (orderTotal < minimumOrder_) {
        return false;
    }
    return maxUses_ == kUnlimitedUses || usedCount_ < maxUses_;
}

void PromoCode::validate(const Money& orderTotal, std::time_t now) const {
    if (!active_) {
        throw InvalidPromoCodeException("Promo code is inactive: " + code_);
    }
    if (now < validFrom_ || now > validUntil_) {
        throw InvalidPromoCodeException("Promo code has expired: " + code_);
    }
    if (orderTotal < minimumOrder_) {
        throw InvalidPromoCodeException(
            "Order total is below the promo minimum of " + minimumOrder_.toString());
    }
    if (maxUses_ != kUnlimitedUses && usedCount_ >= maxUses_) {
        throw InvalidPromoCodeException("Promo code usage limit reached: " + code_);
    }
}

Money PromoCode::discountFor(const Money& orderTotal, std::time_t now) const {
    validate(orderTotal, now);
    return orderTotal * (discountPercent_ / kMaximumPercent);
}

Money PromoCode::redeem(const Money& orderTotal, std::time_t now) {
    const Money discount = discountFor(orderTotal, now);
    ++usedCount_;
    return discount;
}
