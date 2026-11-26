#include "services/PaymentService.hpp"

#include "exceptions/InvalidOrderException.hpp"
#include "exceptions/PaymentFailedException.hpp"
#include "finance/Payment.hpp"
#include "ordering/Order.hpp"
#include "people/Cashier.hpp"

#include <stdexcept>
#include <utility>

void PaymentService::checkout(Order& order, std::unique_ptr<Payment> payment) {
    if (!payment) {
        throw std::invalid_argument("Payment must not be null");
    }
    if (payment->amount() != order.total()) {
        throw InvalidOrderException(
            "Payment amount " + payment->amount().toString() +
            " does not match order total " + order.total().toString());
    }
    order.setPayment(std::move(payment));
    order.checkout();
    revenue_ += order.total();
    ++paymentsCount_;
    if (cashier_ != nullptr) {
        cashier_->registerOrder();
        cashier_->recordPayment(order.total());
    }
}

void PaymentService::refund(Order& order) {
    if (!order.isPaid()) {
        throw PaymentFailedException("Order #" + std::to_string(order.id()) +
                                     " has no completed payment");
    }
    Payment* payment = order.payment();
    const Money amount = payment->amount();
    payment->refund();
    revenue_ = revenue_ - amount;
    ++refundsCount_;
    if (cashier_ != nullptr) {
        cashier_->recordRefund(amount);
    }
}

void PaymentService::setCashier(Cashier* cashier) {
    cashier_ = cashier;
}

Cashier* PaymentService::cashier() const {
    return cashier_;
}

const Money& PaymentService::revenue() const {
    return revenue_;
}

std::size_t PaymentService::paymentsCount() const {
    return paymentsCount_;
}

std::size_t PaymentService::refundsCount() const {
    return refundsCount_;
}
