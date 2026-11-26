#ifndef EMPTY_ORDER_EXCEPTION_HPP
#define EMPTY_ORDER_EXCEPTION_HPP

#include "exceptions/PizzeriaException.hpp"

class EmptyOrderException : public PizzeriaException {
public:
    explicit EmptyOrderException(const std::string& message);
};

#endif
