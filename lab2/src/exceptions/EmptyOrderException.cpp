#include "exceptions/EmptyOrderException.hpp"

namespace {
constexpr int kErrorCode = 1003;
}

EmptyOrderException::EmptyOrderException(const std::string& message)
    : PizzeriaException(message, kErrorCode) {}
