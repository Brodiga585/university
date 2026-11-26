#include "exceptions/PaymentFailedException.hpp"

namespace {
constexpr int kErrorCode = 1007;
}

PaymentFailedException::PaymentFailedException(const std::string& message)
    : PizzeriaException(message, kErrorCode) {}
