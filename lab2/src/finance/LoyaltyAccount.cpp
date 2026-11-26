#include "finance/LoyaltyAccount.hpp"

#include "exceptions/InsufficientFundsException.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
constexpr double kRublesPerPoint = 10.0;
constexpr int kKopeksPerPoint = 50;
constexpr int kNoPoints = 0;
}

LoyaltyAccount::LoyaltyAccount(std::string customerName, int points)
    : customerName_(std::move(customerName)), points_(points), tier_(Tier::Bronze) {
    if (customerName_.empty()) {
        throw std::invalid_argument("Loyalty customer name must not be empty");
    }
    if (points_ < kNoPoints) {
        throw std::invalid_argument("Loyalty points must not be negative");
    }
    updateTier();
}

const std::string& LoyaltyAccount::customerName() const {
    return customerName_;
}

int LoyaltyAccount::points() const {
    return points_;
}

LoyaltyAccount::Tier LoyaltyAccount::tier() const {
    return tier_;
}

const Money& LoyaltyAccount::totalSpent() const {
    return totalSpent_;
}

double LoyaltyAccount::bonusMultiplier() const {
    switch (tier_) {
        case Tier::Bronze:
            return 1.0;
        case Tier::Silver:
            return 1.25;
        case Tier::Gold:
            return 1.5;
        case Tier::Platinum:
            return 2.0;
    }
    return 1.0;
}

void LoyaltyAccount::earn(const Money& purchase) {
    if (purchase.minorUnits() < 0) {
        throw std::invalid_argument("Loyalty purchase amount must not be negative");
    }
    const double basePoints = purchase.toRubles() / kRublesPerPoint;
    const int earned = static_cast<int>(std::floor(basePoints * bonusMultiplier()));
    points_ += earned;
    totalSpent_ += purchase;
    updateTier();
}

Money LoyaltyAccount::redeem(int points) {
    if (points <= kNoPoints) {
        throw std::invalid_argument("Points to redeem must be positive");
    }
    if (points > points_) {
        throw InsufficientFundsException(
            "Not enough loyalty points: requested " + std::to_string(points) +
            ", available " + std::to_string(points_));
    }
    points_ -= points;
    return Money(static_cast<long long>(points) * kKopeksPerPoint);
}

Money LoyaltyAccount::valueOfPoints(int points) const {
    if (points < kNoPoints) {
        throw std::invalid_argument("Points must not be negative");
    }
    return Money(static_cast<long long>(points) * kKopeksPerPoint);
}

std::string LoyaltyAccount::tierLabel(Tier tier) {
    switch (tier) {
        case Tier::Bronze:
            return "Bronze";
        case Tier::Silver:
            return "Silver";
        case Tier::Gold:
            return "Gold";
        case Tier::Platinum:
            return "Platinum";
    }
    return "Unknown";
}

LoyaltyAccount::Tier LoyaltyAccount::tierForSpending(const Money& totalSpent) {
    if (!(totalSpent < Money::fromRubles(5000.0))) {
        if (!(totalSpent < Money::fromRubles(20000.0))) {
            if (!(totalSpent < Money::fromRubles(50000.0))) {
                return Tier::Platinum;
            }
            return Tier::Gold;
        }
        return Tier::Silver;
    }
    return Tier::Bronze;
}

std::string LoyaltyAccount::describe() const {
    std::ostringstream output;
    output << customerName_ << " [" << tierLabel(tier_) << "] " << points_
           << " points, spent " << totalSpent_.toString()
           << ", x" << bonusMultiplier() << " bonus";
    return output.str();
}

void LoyaltyAccount::updateTier() {
    tier_ = tierForSpending(totalSpent_);
}
