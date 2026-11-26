#ifndef SAUCE_HPP
#define SAUCE_HPP

#include "core/Money.hpp"

#include <string>

class Sauce {
public:
    Sauce(std::string name, int spiciness, double priceFactor, bool vegetarian);

    const std::string& name() const;
    int spiciness() const;
    double priceFactor() const;
    bool isVegetarian() const;

    Money surcharge(const Money& basePrice) const;
    bool isSpicy() const;
    std::string label() const;

private:
    std::string name_;
    int spiciness_;
    double priceFactor_;
    bool vegetarian_;
};

#endif
