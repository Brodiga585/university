#ifndef INSUFFICIENT_FUNDS_EXCEPTION_HPP
#define INSUFFICIENT_FUNDS_EXCEPTION_HPP

#include "exceptions/PizzeriaException.hpp"

class InsufficientFundsException : public PizzeriaException {
public:
    explicit InsufficientFundsException(const std::string& message);
};

#endif
