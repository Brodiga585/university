#ifndef PAYMENT_FAILED_EXCEPTION_HPP
#define PAYMENT_FAILED_EXCEPTION_HPP

#include "exceptions/PizzeriaException.hpp"

class PaymentFailedException : public PizzeriaException {
public:
    explicit PaymentFailedException(const std::string& message);
};

#endif
