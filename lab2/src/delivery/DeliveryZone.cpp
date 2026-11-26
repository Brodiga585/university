#include "delivery/DeliveryZone.hpp"

#include "exceptions/DeliveryZoneNotCoveredException.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <utility>

DeliveryZone::DeliveryZone(std::string name,
                           Address center,
                           double minimumDistanceKm,
                           double maximumDistanceKm,
                           const Money& baseFee,
                           const Money& perKmFee,
                           int baseMinutes,
                           double minutesPerKm,
                           const Money& minimumOrder)
    : name_(std::move(name)),
      center_(std::move(center)),
      minimumDistanceKm_(minimumDistanceKm),
      maximumDistanceKm_(maximumDistanceKm),
      baseFee_(baseFee),
      perKmFee_(perKmFee),
      baseMinutes_(baseMinutes),
      minutesPerKm_(minutesPerKm),
      minimumOrder_(minimumOrder) {
    if (name_.empty()) {
        throw std::invalid_argument("Delivery zone name must not be empty");
    }
    if (!std::isfinite(minimumDistanceKm_) || minimumDistanceKm_ < 0.0) {
        throw std::invalid_argument("Minimum zone distance must be finite and non-negative");
    }
    if (!std::isfinite(maximumDistanceKm_) || maximumDistanceKm_ <= minimumDistanceKm_) {
        throw std::invalid_argument("Maximum zone distance must exceed the minimum");
    }
    if (baseFee_.minorUnits() < 0 || perKmFee_.minorUnits() < 0) {
        throw std::invalid_argument("Delivery fees must not be negative");
    }
    if (baseMinutes_ < 0) {
        throw std::invalid_argument("Base delivery time must not be negative");
    }
    if (!std::isfinite(minutesPerKm_) || minutesPerKm_ < 0.0) {
        throw std::invalid_argument("Minutes per kilometer must be finite and non-negative");
    }
    if (minimumOrder_.minorUnits() < 0) {
        throw std::invalid_argument("Delivery minimum order must not be negative");
    }
}

const std::string& DeliveryZone::name() const {
    return name_;
}

const Address& DeliveryZone::center() const {
    return center_;
}

double DeliveryZone::minimumDistanceKm() const {
    return minimumDistanceKm_;
}

double DeliveryZone::maximumDistanceKm() const {
    return maximumDistanceKm_;
}

const Money& DeliveryZone::baseFee() const {
    return baseFee_;
}

const Money& DeliveryZone::perKmFee() const {
    return perKmFee_;
}

int DeliveryZone::baseMinutes() const {
    return baseMinutes_;
}

double DeliveryZone::minutesPerKm() const {
    return minutesPerKm_;
}

const Money& DeliveryZone::minimumOrder() const {
    return minimumOrder_;
}

double DeliveryZone::distanceTo(const Address& destination) const {
    return center_.distanceTo(destination);
}

bool DeliveryZone::covers(const Address& destination) const {
    if (!center_.sameCity(destination)) {
        return false;
    }
    const double distance = distanceTo(destination);
    return distance >= minimumDistanceKm_ && distance <= maximumDistanceKm_;
}

void DeliveryZone::ensureCovered(const Address& destination) const {
    if (!covers(destination)) {
        throw DeliveryZoneNotCoveredException(
            "Address is outside delivery zone '" + name_ + "': " + destination.format());
    }
}

Money DeliveryZone::deliveryFee(const Address& destination) const {
    ensureCovered(destination);
    return baseFee_ + perKmFee_ * distanceTo(destination);
}

int DeliveryZone::estimatedMinutes(const Address& destination) const {
    ensureCovered(destination);
    const double minutes = static_cast<double>(baseMinutes_) +
                           distanceTo(destination) * minutesPerKm_;
    return static_cast<int>(std::lround(minutes));
}

bool DeliveryZone::meetsMinimum(const Money& orderTotal) const {
    return !(orderTotal < minimumOrder_);
}

std::string DeliveryZone::describe() const {
    std::ostringstream output;
    output << "Zone '" << name_ << "' up to " << maximumDistanceKm_ << " km | base "
           << baseFee_.toString() << " + " << perKmFee_.toString()
           << "/km | ETA from " << baseMinutes_ << " min | min order "
           << minimumOrder_.toString();
    return output.str();
}
