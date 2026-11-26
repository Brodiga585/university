#ifndef DOUGH_HPP
#define DOUGH_HPP

#include "core/Money.hpp"

#include <string>

class Dough {
public:
    enum class Type {
        Classic,
        Thin,
        Thick,
        GlutenFree
    };

    Dough(Type type, std::string description, double priceFactor);

    Type type() const;
    const std::string& description() const;
    double priceFactor() const;

    Money surcharge(const Money& basePrice) const;
    bool isGlutenFree() const;
    std::string label() const;

private:
    Type type_;
    std::string description_;
    double priceFactor_;
};

#endif
