#ifndef INVALID_ORDER_EXCEPTION_HPP
#define INVALID_ORDER_EXCEPTION_HPP

#include "exceptions/PizzeriaException.hpp"

class InvalidOrderException : public PizzeriaException {
public:
    explicit InvalidOrderException(const std::string& message);
};

#endif
