#include "finance/Payment.hpp"

#include "exceptions/PaymentFailedException.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

std::string paymentStatusLabel(PaymentStatus status) {
    switch (status) {
        case PaymentStatus::Pending:
            return "Pending";
        case PaymentStatus::Completed:
            return "Completed";
        case PaymentStatus::Refunded:
            return "Refunded";
        case PaymentStatus::Failed:
            return "Failed";
    }
    return "Unknown";
}

Payment::Payment(Money amount) : amount_(std::move(amount)), status_(PaymentStatus::Pending) {
    if (amount_.minorUnits() < 0) {
        throw std::invalid_argument("Payment amount must not be negative");
    }
}

const Money& Payment::amount() const {
    return amount_;
}

PaymentStatus Payment::status() const {
    return status_;
}

bool Payment::isCompleted() const {
    return status_ == PaymentStatus::Completed;
}

void Payment::refund() {
    requireCompleted();
    status_ = PaymentStatus::Refunded;
}

std::string Payment::describe() const {
    std::ostringstream output;
    output << method() << " payment of " << amount_.toString()
           << " [" << paymentStatusLabel(status_) << ']';
    return output.str();
}

void Payment::requirePending() const {
    if (status_ != PaymentStatus::Pending) {
        throw PaymentFailedException("Payment is not pending: " +
                                     paymentStatusLabel(status_));
    }
}

void Payment::requireCompleted() const {
    if (status_ != PaymentStatus::Completed) {
        throw PaymentFailedException("Payment is not completed: " +
                                     paymentStatusLabel(status_));
    }
}

void Payment::markCompleted() {
    status_ = PaymentStatus::Completed;
}
