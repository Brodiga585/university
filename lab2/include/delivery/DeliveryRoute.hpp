#ifndef DELIVERY_ROUTE_HPP
#define DELIVERY_ROUTE_HPP

#include "core/Address.hpp"

#include <cstddef>
#include <string>
#include <vector>

class DeliveryRoute {
public:
    struct Stop {
        Address address;
        std::string label;
    };

    explicit DeliveryRoute(Address start);

    const Address& start() const;
    const std::vector<Stop>& stops() const;
    std::size_t stopCount() const;
    bool empty() const;

    void addStop(Address address, std::string label = {});
    void clear();
    void optimize();

    double totalDistance() const;
    int estimatedMinutes(double averageSpeedKmh, int minutesPerStop) const;
    std::string describe() const;

private:
    Address start_;
    std::vector<Stop> stops_;
};

#endif
