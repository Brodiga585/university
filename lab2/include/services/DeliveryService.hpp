#ifndef DELIVERY_SERVICE_HPP
#define DELIVERY_SERVICE_HPP

#include "core/Entity.hpp"
#include "delivery/CourierAssignment.hpp"
#include "delivery/DeliveryZone.hpp"

#include <cstddef>
#include <vector>

class Address;
class Courier;
class DeliveryOrder;

class DeliveryService {
public:
    DeliveryService() = default;

    DeliveryZone& addZone(DeliveryZone zone);
    const std::vector<DeliveryZone>& zones() const;
    const DeliveryZone* findZoneFor(const Address& address) const;

    void registerCourier(Courier* courier);
    const std::vector<Courier*>& couriers() const;
    Courier* availableCourier() const;

    CourierAssignment& assign(DeliveryOrder& order);
    CourierAssignment& assign(DeliveryOrder& order, Courier& courier);
    CourierAssignment* findAssignment(Entity::Id orderId);
    const CourierAssignment* findAssignment(Entity::Id orderId) const;

    bool dispatch(Entity::Id orderId);
    bool complete(Entity::Id orderId);
    std::size_t activeAssignments() const;
    const std::vector<CourierAssignment>& assignments() const;

private:
    std::vector<DeliveryZone> zones_;
    std::vector<Courier*> couriers_;
    std::vector<CourierAssignment> assignments_;
};

#endif
