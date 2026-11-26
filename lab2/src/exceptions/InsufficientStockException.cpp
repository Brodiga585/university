#include "exceptions/InsufficientStockException.hpp"

namespace {
constexpr int kErrorCode = 1005;
}

InsufficientStockException::InsufficientStockException(const std::string& message)
    : PizzeriaException(message, kErrorCode) {}
