#ifndef TOPPING_HPP
#define TOPPING_HPP

#include "catalog/Ingredient.hpp"
#include "core/Money.hpp"

#include <string>

class Topping {
public:
    Topping(Ingredient ingredient, double portion, const Money& extraPrice);

    const Ingredient& ingredient() const;
    double portion() const;
    const Money& extraPrice() const;

    Money cost() const;
    double calories() const;
    bool isVegetarian() const;
    std::string describe() const;

private:
    Ingredient ingredient_;
    double portion_;
    Money extraPrice_;
};

#endif
