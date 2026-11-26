#include "exceptions/DeliveryZoneNotCoveredException.hpp"

namespace {
constexpr int kErrorCode = 1011;
}

DeliveryZoneNotCoveredException::DeliveryZoneNotCoveredException(const std::string& message)
    : PizzeriaException(message, kErrorCode) {}
