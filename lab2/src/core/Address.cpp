#include "core/Address.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
constexpr double kEarthRadiusKm = 6371.0088;
constexpr double kDegreesToRadians = 3.14159265358979323846 / 180.0;
constexpr double kMinimumLatitude = -90.0;
constexpr double kMaximumLatitude = 90.0;
constexpr double kMinimumLongitude = -180.0;
constexpr double kMaximumLongitude = 180.0;
}

Address::Address(std::string city,
                 std::string street,
                 std::string building,
                 std::string apartment,
                 std::string postalCode,
                 double latitude,
                 double longitude)
    : city_(std::move(city)),
      street_(std::move(street)),
      building_(std::move(building)),
      apartment_(std::move(apartment)),
      postalCode_(std::move(postalCode)),
      latitude_(latitude),
      longitude_(longitude) {
    if (!isValid()) {
        throw std::invalid_argument("Address contains invalid fields or coordinates");
    }
}

const std::string& Address::city() const {
    return city_;
}

const std::string& Address::street() const {
    return street_;
}

const std::string& Address::building() const {
    return building_;
}

const std::string& Address::apartment() const {
    return apartment_;
}

const std::string& Address::postalCode() const {
    return postalCode_;
}

double Address::latitude() const {
    return latitude_;
}

double Address::longitude() const {
    return longitude_;
}

double Address::distanceTo(const Address& other) const {
    const double latitudeDelta = (other.latitude_ - latitude_) * kDegreesToRadians;
    const double longitudeDelta = (other.longitude_ - longitude_) * kDegreesToRadians;
    const double firstLatitude = latitude_ * kDegreesToRadians;
    const double secondLatitude = other.latitude_ * kDegreesToRadians;

    const double latitudePart = std::sin(latitudeDelta / 2.0);
    const double longitudePart = std::sin(longitudeDelta / 2.0);
    const double haversine = latitudePart * latitudePart +
                             std::cos(firstLatitude) * std::cos(secondLatitude) *
                                 longitudePart * longitudePart;
    const double angle = 2.0 * std::atan2(std::sqrt(haversine), std::sqrt(1.0 - haversine));
    return kEarthRadiusKm * angle;
}

std::string Address::format() const {
    std::ostringstream output;
    output << city_ << ", " << street_ << ' ' << building_;
    if (!apartment_.empty()) {
        output << ", apt " << apartment_;
    }
    if (!postalCode_.empty()) {
        output << ", " << postalCode_;
    }
    return output.str();
}

bool Address::isValid() const {
    const bool fieldsValid = !city_.empty() && !street_.empty() && !building_.empty();
    const bool latitudeValid = latitude_ >= kMinimumLatitude && latitude_ <= kMaximumLatitude;
    const bool longitudeValid = longitude_ >= kMinimumLongitude && longitude_ <= kMaximumLongitude;
    return fieldsValid && latitudeValid && longitudeValid;
}

bool Address::sameCity(const Address& other) const {
    return city_ == other.city_;
}
