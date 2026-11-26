#include "exceptions/MenuItemUnavailableException.hpp"

namespace {
constexpr int kErrorCode = 1004;
}

MenuItemUnavailableException::MenuItemUnavailableException(const std::string& message)
    : PizzeriaException(message, kErrorCode) {}
