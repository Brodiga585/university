#ifndef CARD_PAYMENT_HPP
#define CARD_PAYMENT_HPP

#include "finance/Payment.hpp"

#include <string>

class CardPayment : public Payment {
public:
    CardPayment(Money amount,
                std::string cardNumber,
                std::string holder,
                int expiryMonth,
                int expiryYear,
                bool approved = true);

    const std::string& holder() const;
    std::string maskedNumber() const;
    int expiryMonth() const;
    int expiryYear() const;
    bool isApproved() const;
    void setApproved(bool approved);

    std::string method() const override;
    void pay() override;

private:
    static void validateCardNumber(const std::string& cardNumber);

    std::string cardNumber_;
    std::string holder_;
    int expiryMonth_;
    int expiryYear_;
    bool approved_;
};

#endif
