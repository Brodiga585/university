#ifndef PIZZA_SIZE_HPP
#define PIZZA_SIZE_HPP

#include "core/Money.hpp"

#include <string>

class PizzaSize {
public:
    PizzaSize(std::string name, double diameterCm, double priceMultiplier, int slices);

    const std::string& name() const;
    double diameterCm() const;
    double priceMultiplier() const;
    int slices() const;

    Money priceFor(const Money& basePrice) const;
    double area() const;
    double sliceArea() const;
    std::string label() const;

private:
    std::string name_;
    double diameterCm_;
    double priceMultiplier_;
    int slices_;
};

#endif
