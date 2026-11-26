#ifndef PAYMENT_SERVICE_HPP
#define PAYMENT_SERVICE_HPP

#include "core/Money.hpp"

#include <cstddef>
#include <memory>

class Order;
class Payment;
class Cashier;

class PaymentService {
public:
    PaymentService() = default;

    void checkout(Order& order, std::unique_ptr<Payment> payment);
    void refund(Order& order);

    void setCashier(Cashier* cashier);
    Cashier* cashier() const;

    const Money& revenue() const;
    std::size_t paymentsCount() const;
    std::size_t refundsCount() const;

private:
    Money revenue_;
    Cashier* cashier_ = nullptr;
    std::size_t paymentsCount_ = 0;
    std::size_t refundsCount_ = 0;
};

#endif
