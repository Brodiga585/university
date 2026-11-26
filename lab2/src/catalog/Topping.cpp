#include "catalog/Topping.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <utility>

Topping::Topping(Ingredient ingredient, double portion, const Money& extraPrice)
    : ingredient_(std::move(ingredient)), portion_(portion), extraPrice_(extraPrice) {
    if (!std::isfinite(portion_) || portion_ <= 0.0) {
        throw std::invalid_argument("Topping portion must be finite and positive");
    }
    if (extraPrice_.minorUnits() < 0) {
        throw std::invalid_argument("Topping price must not be negative");
    }
}

const Ingredient& Topping::ingredient() const {
    return ingredient_;
}

double Topping::portion() const {
    return portion_;
}

const Money& Topping::extraPrice() const {
    return extraPrice_;
}

Money Topping::cost() const {
    return ingredient_.costFor(portion_);
}

double Topping::calories() const {
    return ingredient_.caloriesFor(portion_);
}

bool Topping::isVegetarian() const {
    return ingredient_.isVegetarian();
}

std::string Topping::describe() const {
    std::ostringstream output;
    output << ingredient_.name() << ' ' << portion_ << ' ' << ingredient_.unitLabel()
           << " (+" << extraPrice_.toString() << ')';
    return output.str();
}
