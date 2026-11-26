#include "finance/OnlinePayment.hpp"

#include "exceptions/PaymentFailedException.hpp"

#include <atomic>
#include <stdexcept>
#include <utility>

namespace {
std::atomic<long long> nextTransactionNumber{1};

std::string makeTransactionId(const std::string& provider) {
    const long long number = nextTransactionNumber.fetch_add(1, std::memory_order_relaxed);
    return provider + "-" + std::to_string(number);
}
}

OnlinePayment::OnlinePayment(Money amount,
                             std::string provider,
                             std::string account,
                             bool confirmed)
    : Payment(std::move(amount)),
      provider_(std::move(provider)),
      account_(std::move(account)),
      confirmed_(confirmed) {
    if (provider_.empty()) {
        throw std::invalid_argument("Payment provider must not be empty");
    }
    if (account_.empty()) {
        throw std::invalid_argument("Payment account must not be empty");
    }
}

const std::string& OnlinePayment::provider() const {
    return provider_;
}

const std::string& OnlinePayment::account() const {
    return account_;
}

const std::string& OnlinePayment::transactionId() const {
    return transactionId_;
}

bool OnlinePayment::isConfirmed() const {
    return confirmed_;
}

void OnlinePayment::setConfirmed(bool confirmed) {
    confirmed_ = confirmed;
}

std::string OnlinePayment::method() const {
    return "Online";
}

void OnlinePayment::pay() {
    requirePending();
    if (!confirmed_) {
        status_ = PaymentStatus::Failed;
        throw PaymentFailedException("Online payment was not confirmed by " + provider_);
    }
    transactionId_ = makeTransactionId(provider_);
    markCompleted();
}
