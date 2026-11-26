#include "exceptions/InvalidOrderException.hpp"

namespace {
constexpr int kErrorCode = 1001;
}

InvalidOrderException::InvalidOrderException(const std::string& message)
    : PizzeriaException(message, kErrorCode) {}
