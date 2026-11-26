#ifndef SUPPLIER_HPP
#define SUPPLIER_HPP

#include "core/Address.hpp"
#include "core/Money.hpp"

#include <string>
#include <vector>

class Supplier {
public:
    enum class Weekday {
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
        Sunday
    };

    Supplier(std::string name,
             std::string contactPerson,
             std::string phone,
             Address address,
             std::vector<Weekday> deliveryDays,
             double rating,
             const Money& minimumOrder);

    const std::string& name() const;
    const std::string& contactPerson() const;
    const std::string& phone() const;
    const Address& address() const;
    const std::vector<Weekday>& deliveryDays() const;
    double rating() const;
    const Money& minimumOrder() const;

    bool deliversOn(Weekday weekday) const;
    bool meetsMinimum(const Money& amount) const;
    std::string weekdayLabel(Weekday weekday) const;
    std::string describe() const;

private:
    std::string name_;
    std::string contactPerson_;
    std::string phone_;
    Address address_;
    std::vector<Weekday> deliveryDays_;
    double rating_;
    Money minimumOrder_;
};

#endif
