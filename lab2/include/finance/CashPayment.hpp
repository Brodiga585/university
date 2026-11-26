#ifndef CASH_PAYMENT_HPP
#define CASH_PAYMENT_HPP

#include "finance/Payment.hpp"

class CashPayment : public Payment {
public:
    CashPayment(Money amount, Money tendered);

    const Money& tendered() const;
    Money change() const;

    std::string method() const override;
    void pay() override;

private:
    Money tendered_;
};

#endif
