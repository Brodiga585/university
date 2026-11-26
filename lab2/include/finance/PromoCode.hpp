#ifndef PROMO_CODE_HPP
#define PROMO_CODE_HPP

#include "core/Money.hpp"

#include <ctime>
#include <string>

class PromoCode {
public:
    PromoCode(std::string code,
              double discountPercent,
              const Money& minimumOrder,
              std::time_t validFrom,
              std::time_t validUntil,
              int maxUses = 0);

    const std::string& code() const;
    double discountPercent() const;
    const Money& minimumOrder() const;
    std::time_t validFrom() const;
    std::time_t validUntil() const;
    int maxUses() const;
    int usedCount() const;
    int remainingUses() const;
    bool isActive() const;

    void setActive(bool active);
    bool isValidFor(const Money& orderTotal, std::time_t now) const;
    void validate(const Money& orderTotal, std::time_t now) const;
    Money discountFor(const Money& orderTotal, std::time_t now) const;
    Money redeem(const Money& orderTotal, std::time_t now);

private:
    std::string code_;
    double discountPercent_;
    Money minimumOrder_;
    std::time_t validFrom_;
    std::time_t validUntil_;
    int maxUses_;
    int usedCount_;
    bool active_;
};

#endif
