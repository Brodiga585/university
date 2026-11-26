#include "catalog/Pizza.hpp"

#include <algorithm>
#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <utility>

Pizza::Pizza(std::string name,
             std::string description,
             const Money& basePrice,
             PizzaSize size,
             Dough dough,
             Sauce sauce,
             Recipe recipe,
             std::vector<Topping> toppings)
    : MenuItem(std::move(name), std::move(description), basePrice),
      size_(std::move(size)),
      dough_(std::move(dough)),
      sauce_(std::move(sauce)),
      toppings_(std::move(toppings)),
      recipe_(std::move(recipe)) {
    recipe_.validate();
}

const PizzaSize& Pizza::size() const {
    return size_;
}

const Dough& Pizza::dough() const {
    return dough_;
}

const Sauce& Pizza::sauce() const {
    return sauce_;
}

const Recipe& Pizza::recipe() const {
    return recipe_;
}

const std::vector<Topping>& Pizza::toppings() const {
    return toppings_;
}

void Pizza::addTopping(const Topping& topping) {
    const auto existing = std::any_of(
        toppings_.begin(), toppings_.end(),
        [&topping](const Topping& current) {
            return current.ingredient().name() == topping.ingredient().name();
        });
    if (existing) {
        throw std::invalid_argument("Topping already added: " + topping.ingredient().name());
    }
    toppings_.push_back(topping);
}

bool Pizza::removeTopping(const std::string& name) {
    const auto newEnd = std::remove_if(
        toppings_.begin(), toppings_.end(),
        [&name](const Topping& topping) {
            return topping.ingredient().name() == name;
        });
    if (newEnd == toppings_.end()) {
        return false;
    }
    toppings_.erase(newEnd, toppings_.end());
    return true;
}

Money Pizza::calculatePrice() const {
    Money extras = dough_.surcharge(basePrice_) + sauce_.surcharge(basePrice_);
    for (const auto& topping : toppings_) {
        extras += topping.extraPrice();
    }
    return size_.priceFor(basePrice_ + extras);
}

double Pizza::calculateCalories() const {
    double total = recipe_.totalCalories();
    for (const auto& topping : toppings_) {
        total += topping.calories();
    }
    return total;
}

Money Pizza::foodCost() const {
    Money total = recipe_.totalCost();
    for (const auto& topping : toppings_) {
        total += topping.cost();
    }
    return total;
}

std::string Pizza::category() const {
    return "Pizza";
}

std::unique_ptr<MenuItem> Pizza::clone() const {
    return std::make_unique<Pizza>(
        name_, description_, basePrice_, size_, dough_, sauce_, recipe_, toppings_);
}

std::string Pizza::describe() const {
    std::ostringstream output;
    output << MenuItem::describe()
           << " | " << size_.label()
           << ", " << dough_.label()
           << ", " << sauce_.label();
    if (!toppings_.empty()) {
        output << " | toppings: ";
        for (std::size_t index = 0; index < toppings_.size(); ++index) {
            if (index > 0) {
                output << ", ";
            }
            output << toppings_[index].ingredient().name();
        }
    }
    return output.str();
}
