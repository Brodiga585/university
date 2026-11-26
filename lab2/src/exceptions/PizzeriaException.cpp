#include "exceptions/PizzeriaException.hpp"

PizzeriaException::PizzeriaException(const std::string& message, int code)
    : std::runtime_error(message), code_(code) {}

int PizzeriaException::code() const {
    return code_;
}
