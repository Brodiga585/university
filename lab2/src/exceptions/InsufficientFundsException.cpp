#include "exceptions/InsufficientFundsException.hpp"

namespace {
constexpr int kErrorCode = 1008;
}

InsufficientFundsException::InsufficientFundsException(const std::string& message)
    : PizzeriaException(message, kErrorCode) {}
