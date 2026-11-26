#ifndef COURIER_HPP
#define COURIER_HPP

#include "people/Employee.hpp"

#include <string>

class Courier : public Employee {
public:
    enum class Vehicle {
        Foot,
        Bicycle,
        Scooter,
        Car
    };

    Courier(std::string fullName,
            std::string phone,
            std::string email,
            const Money& monthlySalary,
            std::time_t hiredAt,
            Vehicle vehicle,
            std::string transportNumber = {});

    Vehicle vehicle() const;
    const std::string& transportNumber() const;
    bool isAvailable() const;
    int completedDeliveries() const;

    void startDelivery();
    void completeDelivery();

    std::string vehicleLabel() const;
    std::string role() const override;
    std::string describe() const override;

private:
    Vehicle vehicle_;
    std::string transportNumber_;
    bool available_;
    int completedDeliveries_;
};

#endif
