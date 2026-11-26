#ifndef RESERVATION_CONFLICT_EXCEPTION_HPP
#define RESERVATION_CONFLICT_EXCEPTION_HPP

#include "exceptions/PizzeriaException.hpp"

class ReservationConflictException : public PizzeriaException {
public:
    explicit ReservationConflictException(const std::string& message);
};

#endif
