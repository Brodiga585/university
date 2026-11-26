#ifndef DELIVERY_ZONE_NOT_COVERED_EXCEPTION_HPP
#define DELIVERY_ZONE_NOT_COVERED_EXCEPTION_HPP

#include "exceptions/PizzeriaException.hpp"

class DeliveryZoneNotCoveredException : public PizzeriaException {
public:
    explicit DeliveryZoneNotCoveredException(const std::string& message);
};

#endif
