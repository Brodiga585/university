#include "exceptions/TableNotAvailableException.hpp"

namespace {
constexpr int kErrorCode = 1009;
}

TableNotAvailableException::TableNotAvailableException(const std::string& message)
    : PizzeriaException(message, kErrorCode) {}
