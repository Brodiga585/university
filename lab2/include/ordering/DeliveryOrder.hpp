#ifndef DELIVERY_ORDER_HPP
#define DELIVERY_ORDER_HPP

#include "core/Address.hpp"
#include "ordering/Order.hpp"

#include <ctime>

class Courier;
class DeliveryZone;

class DeliveryOrder : public Order {
public:
    explicit DeliveryOrder(std::string customerName,
                           Address address,
                           const DeliveryZone* zone = nullptr,
                           std::time_t promisedTime = 0);

    const Address& address() const;
    const DeliveryZone* zone() const;
    Courier* courier() const;
    std::time_t promisedTime() const;
    std::time_t dispatchedAt() const;
    std::time_t deliveredAt() const;

    void setZone(const DeliveryZone* zone);
    void assignCourier(Courier* courier);
    void clearCourier();
    void setPromisedTime(std::time_t promisedTime);

    Money deliveryFee() const;
    int estimatedMinutes() const;
    bool isLate(std::time_t now) const;

    Money extraFee() const override;
    void dispatch() override;
    void complete() override;
    std::string orderType() const override;
    std::string describe() const override;

private:
    Address address_;
    const DeliveryZone* zone_;
    Courier* courier_;
    std::time_t promisedTime_;
    std::time_t dispatchedAt_;
    std::time_t deliveredAt_;
};

#endif
