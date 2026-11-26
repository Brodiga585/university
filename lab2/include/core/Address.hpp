#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#include <string>

class Address {
public:
    Address(std::string city,
            std::string street,
            std::string building,
            std::string apartment,
            std::string postalCode,
            double latitude,
            double longitude);

    const std::string& city() const;
    const std::string& street() const;
    const std::string& building() const;
    const std::string& apartment() const;
    const std::string& postalCode() const;
    double latitude() const;
    double longitude() const;

    double distanceTo(const Address& other) const;
    std::string format() const;
    bool isValid() const;
    bool sameCity(const Address& other) const;

private:
    std::string city_;
    std::string street_;
    std::string building_;
    std::string apartment_;
    std::string postalCode_;
    double latitude_;
    double longitude_;
};

#endif
