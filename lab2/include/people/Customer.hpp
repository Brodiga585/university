#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include "core/Address.hpp"
#include "core/Money.hpp"
#include "people/Person.hpp"

#include <optional>
#include <string>

class Customer : public Person {
public:
    Customer(std::string fullName,
             std::string phone,
             std::string email,
             std::optional<Address> address = std::nullopt);

    const std::optional<Address>& address() const;
    int ordersPlaced() const;
    const Money& totalSpent() const;

    void setAddress(Address address);
    void registerOrder();
    void addSpending(const Money& amount);

    std::string role() const override;
    std::string describe() const override;

private:
    std::optional<Address> address_;
    int ordersPlaced_;
    Money totalSpent_;
};

#endif
