#ifndef ORDER_NOT_FOUND_EXCEPTION_HPP
#define ORDER_NOT_FOUND_EXCEPTION_HPP

#include "exceptions/PizzeriaException.hpp"

class OrderNotFoundException : public PizzeriaException {
public:
    explicit OrderNotFoundException(const std::string& message);
};

#endif
