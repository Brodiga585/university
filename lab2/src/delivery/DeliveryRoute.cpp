#include "delivery/DeliveryRoute.hpp"

#include <cmath>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
constexpr double kMinutesPerHour = 60.0;
}

DeliveryRoute::DeliveryRoute(Address start) : start_(std::move(start)) {}

const Address& DeliveryRoute::start() const {
    return start_;
}

const std::vector<DeliveryRoute::Stop>& DeliveryRoute::stops() const {
    return stops_;
}

std::size_t DeliveryRoute::stopCount() const {
    return stops_.size();
}

bool DeliveryRoute::empty() const {
    return stops_.empty();
}

void DeliveryRoute::addStop(Address address, std::string label) {
    stops_.push_back({std::move(address), std::move(label)});
}

void DeliveryRoute::clear() {
    stops_.clear();
}

void DeliveryRoute::optimize() {
    if (stops_.size() < 2) {
        return;
    }
    std::vector<Stop> remaining = stops_;
    std::vector<Stop> ordered;
    ordered.reserve(remaining.size());
    Address current = start_;

    while (!remaining.empty()) {
        std::size_t bestIndex = 0;
        double bestDistance = std::numeric_limits<double>::max();
        for (std::size_t index = 0; index < remaining.size(); ++index) {
            const double distance = current.distanceTo(remaining[index].address);
            if (distance < bestDistance) {
                bestDistance = distance;
                bestIndex = index;
            }
        }
        current = remaining[bestIndex].address;
        ordered.push_back(std::move(remaining[bestIndex]));
        remaining.erase(remaining.begin() + static_cast<std::ptrdiff_t>(bestIndex));
    }
    stops_ = std::move(ordered);
}

double DeliveryRoute::totalDistance() const {
    double total = 0.0;
    Address current = start_;
    for (const auto& stop : stops_) {
        total += current.distanceTo(stop.address);
        current = stop.address;
    }
    return total;
}

int DeliveryRoute::estimatedMinutes(double averageSpeedKmh, int minutesPerStop) const {
    if (!std::isfinite(averageSpeedKmh) || averageSpeedKmh <= 0.0) {
        throw std::invalid_argument("Average speed must be finite and positive");
    }
    if (minutesPerStop < 0) {
        throw std::invalid_argument("Minutes per stop must not be negative");
    }
    const double travelMinutes = totalDistance() / averageSpeedKmh * kMinutesPerHour;
    const double stopMinutes = static_cast<double>(minutesPerStop) *
                               static_cast<double>(stops_.size());
    return static_cast<int>(std::lround(travelMinutes + stopMinutes));
}

std::string DeliveryRoute::describe() const {
    std::ostringstream output;
    output << "Route from " << start_.format() << " with " << stops_.size()
           << " stops, " << totalDistance() << " km";
    for (std::size_t index = 0; index < stops_.size(); ++index) {
        output << "\n  " << (index + 1) << ". ";
        if (!stops_[index].label.empty()) {
            output << stops_[index].label << " - ";
        }
        output << stops_[index].address.format();
    }
    return output.str();
}
