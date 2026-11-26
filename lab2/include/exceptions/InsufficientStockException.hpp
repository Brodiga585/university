#ifndef INSUFFICIENT_STOCK_EXCEPTION_HPP
#define INSUFFICIENT_STOCK_EXCEPTION_HPP

#include "exceptions/PizzeriaException.hpp"

class InsufficientStockException : public PizzeriaException {
public:
    explicit InsufficientStockException(const std::string& message);
};

#endif
