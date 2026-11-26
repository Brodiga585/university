#ifndef CASHIER_HPP
#define CASHIER_HPP

#include "people/Employee.hpp"

#include <cstddef>

class Cashier : public Employee {
public:
    Cashier(std::string fullName,
            std::string phone,
            std::string email,
            const Money& monthlySalary,
            std::time_t hiredAt);

    int processedOrders() const;
    const Money& collected() const;

    void registerOrder();
    void recordPayment(const Money& amount);
    void recordRefund(const Money& amount);

    std::string role() const override;
    std::string describe() const override;

private:
    int processedOrders_;
    Money collected_;
};

#endif
