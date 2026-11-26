#ifndef LOYALTY_ACCOUNT_HPP
#define LOYALTY_ACCOUNT_HPP

#include "core/Money.hpp"

#include <string>

class LoyaltyAccount {
public:
    enum class Tier {
        Bronze,
        Silver,
        Gold,
        Platinum
    };

    LoyaltyAccount(std::string customerName, int points = 0);

    const std::string& customerName() const;
    int points() const;
    Tier tier() const;
    const Money& totalSpent() const;
    double bonusMultiplier() const;

    void earn(const Money& purchase);
    Money redeem(int points);
    Money valueOfPoints(int points) const;

    static std::string tierLabel(Tier tier);
    static Tier tierForSpending(const Money& totalSpent);
    std::string describe() const;

private:
    void updateTier();

    std::string customerName_;
    int points_;
    Tier tier_;
    Money totalSpent_;
};

#endif
