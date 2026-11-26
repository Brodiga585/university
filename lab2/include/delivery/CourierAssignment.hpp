#ifndef COURIER_ASSIGNMENT_HPP
#define COURIER_ASSIGNMENT_HPP

#include <ctime>
#include <string>

class Courier;
class DeliveryOrder;

class CourierAssignment {
public:
    enum class Status {
        Assigned,
        OnTheWay,
        Delivered,
        Cancelled
    };

    CourierAssignment(Courier* courier, DeliveryOrder* order);

    Courier* courier() const;
    DeliveryOrder* order() const;
    Status status() const;
    bool isActive() const;

    std::time_t assignedAt() const;
    std::time_t pickedUpAt() const;
    std::time_t deliveredAt() const;

    void confirmPickup();
    void complete();
    void cancel();

    int durationMinutes(std::time_t now) const;
    bool isLate(std::time_t now) const;
    std::string statusLabel() const;
    std::string describe() const;

private:
    Courier* courier_;
    DeliveryOrder* order_;
    Status status_;
    std::time_t assignedAt_;
    std::time_t pickedUpAt_;
    std::time_t deliveredAt_;
};

#endif
