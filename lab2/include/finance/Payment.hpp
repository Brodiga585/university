#ifndef PAYMENT_HPP
#define PAYMENT_HPP

#include "core/Money.hpp"

#include <string>

enum class PaymentStatus {
    Pending,
    Completed,
    Refunded,
    Failed
};

std::string paymentStatusLabel(PaymentStatus status);

class Payment {
public:
    explicit Payment(Money amount);
    virtual ~Payment() = default;

    const Money& amount() const;
    PaymentStatus status() const;
    bool isCompleted() const;

    virtual std::string method() const = 0;
    virtual void pay() = 0;
    virtual void refund();

    std::string describe() const;

protected:
    void requirePending() const;
    void requireCompleted() const;
    void markCompleted();

    Money amount_;
    PaymentStatus status_;
};

#endif
