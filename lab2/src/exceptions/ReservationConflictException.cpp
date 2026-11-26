#include "exceptions/ReservationConflictException.hpp"

namespace {
constexpr int kErrorCode = 1010;
}

ReservationConflictException::ReservationConflictException(const std::string& message)
    : PizzeriaException(message, kErrorCode) {}
