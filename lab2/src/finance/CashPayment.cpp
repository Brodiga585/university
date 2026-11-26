#include "finance/CashPayment.hpp"

#include "exceptions/InsufficientFundsException.hpp"

#include <stdexcept>
#include <utility>

CashPayment::CashPayment(Money amount, Money tendered)
    : Payment(std::move(amount)), tendered_(std::move(tendered)) {
    if (tendered_.minorUnits() < 0) {
        throw std::invalid_argument("Tendered amount must not be negative");
    }
}

const Money& CashPayment::tendered() const {
    return tendered_;
}

Money CashPayment::change() const {
    if (!isCompleted() && status() != PaymentStatus::Refunded) {
        throw std::logic_error("Change is available only after payment");
    }
    return tendered_ - amount_;
}

std::string CashPayment::method() const {
    return "Cash";
}

void CashPayment::pay() {
    requirePending();
    if (tendered_ < amount_) {
        throw InsufficientFundsException(
            "Insufficient cash: required " + amount_.toString() + ", received " +
            tendered_.toString());
    }
    markCompleted();
}
