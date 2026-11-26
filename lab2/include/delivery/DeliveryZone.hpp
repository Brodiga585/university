#ifndef DELIVERY_ZONE_HPP
#define DELIVERY_ZONE_HPP

#include "core/Address.hpp"
#include "core/Money.hpp"

#include <string>

class DeliveryZone {
public:
    DeliveryZone(std::string name,
                 Address center,
                 double minimumDistanceKm,
                 double maximumDistanceKm,
                 const Money& baseFee,
                 const Money& perKmFee,
                 int baseMinutes,
                 double minutesPerKm,
                 const Money& minimumOrder = Money());

    const std::string& name() const;
    const Address& center() const;
    double minimumDistanceKm() const;
    double maximumDistanceKm() const;
    const Money& baseFee() const;
    const Money& perKmFee() const;
    int baseMinutes() const;
    double minutesPerKm() const;
    const Money& minimumOrder() const;

    double distanceTo(const Address& destination) const;
    bool covers(const Address& destination) const;
    void ensureCovered(const Address& destination) const;
    Money deliveryFee(const Address& destination) const;
    int estimatedMinutes(const Address& destination) const;
    bool meetsMinimum(const Money& orderTotal) const;
    std::string describe() const;

private:
    std::string name_;
    Address center_;
    double minimumDistanceKm_;
    double maximumDistanceKm_;
    Money baseFee_;
    Money perKmFee_;
    int baseMinutes_;
    double minutesPerKm_;
    Money minimumOrder_;
};

#endif
