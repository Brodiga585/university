#include "exceptions/InvalidPromoCodeException.hpp"

namespace {
constexpr int kErrorCode = 1006;
}

InvalidPromoCodeException::InvalidPromoCodeException(const std::string& message)
    : PizzeriaException(message, kErrorCode) {}
