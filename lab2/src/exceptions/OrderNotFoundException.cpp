#include "exceptions/OrderNotFoundException.hpp"

namespace {
constexpr int kErrorCode = 1002;
}

OrderNotFoundException::OrderNotFoundException(const std::string& message)
    : PizzeriaException(message, kErrorCode) {}
