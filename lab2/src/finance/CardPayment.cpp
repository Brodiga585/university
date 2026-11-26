#include "finance/CardPayment.hpp"

#include "exceptions/PaymentFailedException.hpp"

#include <cctype>
#include <stdexcept>
#include <utility>

namespace {
constexpr std::size_t kCardNumberLength = 16;
constexpr int kMinimumMonth = 1;
constexpr int kMaximumMonth = 12;
constexpr int kMinimumYear = 2000;
constexpr int kMaximumYear = 2100;
}

CardPayment::CardPayment(Money amount,
                         std::string cardNumber,
                         std::string holder,
                         int expiryMonth,
                         int expiryYear,
                         bool approved)
    : Payment(std::move(amount)),
      cardNumber_(std::move(cardNumber)),
      holder_(std::move(holder)),
      expiryMonth_(expiryMonth),
      expiryYear_(expiryYear),
      approved_(approved) {
    validateCardNumber(cardNumber_);
    if (holder_.empty()) {
        throw std::invalid_argument("Card holder must not be empty");
    }
    if (expiryMonth_ < kMinimumMonth || expiryMonth_ > kMaximumMonth) {
        throw std::invalid_argument("Card expiry month must be between 1 and 12");
    }
    if (expiryYear_ < kMinimumYear || expiryYear_ > kMaximumYear) {
        throw std::invalid_argument("Card expiry year is out of range");
    }
}

const std::string& CardPayment::holder() const {
    return holder_;
}

std::string CardPayment::maskedNumber() const {
    return "**** **** **** " + cardNumber_.substr(cardNumber_.size() - 4);
}

int CardPayment::expiryMonth() const {
    return expiryMonth_;
}

int CardPayment::expiryYear() const {
    return expiryYear_;
}

bool CardPayment::isApproved() const {
    return approved_;
}

void CardPayment::setApproved(bool approved) {
    approved_ = approved;
}

std::string CardPayment::method() const {
    return "Card";
}

void CardPayment::pay() {
    requirePending();
    if (!approved_) {
        status_ = PaymentStatus::Failed;
        throw PaymentFailedException("Card payment was declined by the bank");
    }
    markCompleted();
}

void CardPayment::validateCardNumber(const std::string& cardNumber) {
    if (cardNumber.size() != kCardNumberLength) {
        throw std::invalid_argument("Card number must contain 16 digits");
    }
    for (const char symbol : cardNumber) {
        if (std::isdigit(static_cast<unsigned char>(symbol)) == 0) {
            throw std::invalid_argument("Card number must contain only digits");
        }
    }
}
