#ifndef TAKEAWAY_ORDER_HPP
#define TAKEAWAY_ORDER_HPP

#include "ordering/Order.hpp"

#include <ctime>

class TakeawayOrder : public Order {
public:
    explicit TakeawayOrder(std::string customerName,
                           std::string pickupCode = {},
                           std::time_t readyAt = 0,
                           Money packagingFee = Money::fromRubles(30.0));

    const std::string& pickupCode() const;
    std::time_t readyAt() const;
    std::time_t pickedUpAt() const;
    const Money& packagingFee() const;

    void setReadyAt(std::time_t readyAt);
    void setPackagingFee(const Money& packagingFee);

    Money extraFee() const override;
    void complete() override;
    std::string orderType() const override;
    std::string describe() const override;

private:
    std::string pickupCode_;
    std::time_t readyAt_;
    std::time_t pickedUpAt_;
    Money packagingFee_;
};

#endif
