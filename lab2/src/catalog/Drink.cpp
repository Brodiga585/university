#include "catalog/Drink.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
constexpr double kMillilitersPerHundred = 100.0;
}

Drink::Drink(std::string name,
             std::string description,
             const Money& basePrice,
             int volumeMl,
             std::string brand,
             bool alcoholic,
             double caloriesPer100Ml,
             bool available)
    : MenuItem(std::move(name), std::move(description), basePrice, available),
      volumeMl_(volumeMl),
      brand_(std::move(brand)),
      alcoholic_(alcoholic),
      caloriesPer100Ml_(caloriesPer100Ml) {
    if (volumeMl_ <= 0) {
        throw std::invalid_argument("Drink volume must be positive");
    }
    if (brand_.empty()) {
        throw std::invalid_argument("Drink brand must not be empty");
    }
    if (!std::isfinite(caloriesPer100Ml_) || caloriesPer100Ml_ < 0.0) {
        throw std::invalid_argument("Drink calories must be finite and non-negative");
    }
}

int Drink::volumeMl() const {
    return volumeMl_;
}

const std::string& Drink::brand() const {
    return brand_;
}

bool Drink::isAlcoholic() const {
    return alcoholic_;
}

Money Drink::calculatePrice() const {
    return basePrice_;
}

double Drink::calculateCalories() const {
    return static_cast<double>(volumeMl_) / kMillilitersPerHundred * caloriesPer100Ml_;
}

std::string Drink::category() const {
    return "Drink";
}

std::unique_ptr<MenuItem> Drink::clone() const {
    return std::make_unique<Drink>(
        name_, description_, basePrice_, volumeMl_, brand_, alcoholic_, caloriesPer100Ml_, available_);
}

std::string Drink::describe() const {
    std::ostringstream output;
    output << MenuItem::describe()
           << " | " << volumeMl_ << " ml, " << brand_;
    if (alcoholic_) {
        output << ", alcoholic";
    }
    return output.str();
}
