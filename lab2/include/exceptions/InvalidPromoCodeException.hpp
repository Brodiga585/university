#ifndef INVALID_PROMO_CODE_EXCEPTION_HPP
#define INVALID_PROMO_CODE_EXCEPTION_HPP

#include "exceptions/PizzeriaException.hpp"

class InvalidPromoCodeException : public PizzeriaException {
public:
    explicit InvalidPromoCodeException(const std::string& message);
};

#endif
