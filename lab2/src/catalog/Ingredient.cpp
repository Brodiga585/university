#include "catalog/Ingredient.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <utility>

namespace {
bool equalsIgnoreCase(const std::string& left, const std::string& right) {
    if (left.size() != right.size()) {
        return false;
    }
    return std::equal(left.begin(), left.end(), right.begin(),
                      [](char leftChar, char rightChar) {
                          const auto leftLower = std::tolower(static_cast<unsigned char>(leftChar));
                          const auto rightLower = std::tolower(static_cast<unsigned char>(rightChar));
                          return leftLower == rightLower;
                      });
}
}

Ingredient::Ingredient(std::string name,
                       Unit unit,
                       const Money& costPerUnit,
                       double caloriesPerUnit,
                       std::vector<std::string> allergens,
                       bool vegetarian)
    : name_(std::move(name)),
      unit_(unit),
      costPerUnit_(costPerUnit),
      caloriesPerUnit_(caloriesPerUnit),
      allergens_(std::move(allergens)),
      vegetarian_(vegetarian) {
    if (name_.empty()) {
        throw std::invalid_argument("Ingredient name must not be empty");
    }
    if (costPerUnit_.minorUnits() < 0) {
        throw std::invalid_argument("Ingredient cost must not be negative");
    }
    if (!std::isfinite(caloriesPerUnit_) || caloriesPerUnit_ < 0.0) {
        throw std::invalid_argument("Ingredient calories must be finite and non-negative");
    }
}

const std::string& Ingredient::name() const {
    return name_;
}

Ingredient::Unit Ingredient::unit() const {
    return unit_;
}

const Money& Ingredient::costPerUnit() const {
    return costPerUnit_;
}

double Ingredient::caloriesPerUnit() const {
    return caloriesPerUnit_;
}

bool Ingredient::isVegetarian() const {
    return vegetarian_;
}

Money Ingredient::costFor(double amount) const {
    if (!std::isfinite(amount) || amount < 0.0) {
        throw std::invalid_argument("Ingredient amount must be finite and non-negative");
    }
    return costPerUnit_ * amount;
}

double Ingredient::caloriesFor(double amount) const {
    if (!std::isfinite(amount) || amount < 0.0) {
        throw std::invalid_argument("Ingredient amount must be finite and non-negative");
    }
    return caloriesPerUnit_ * amount;
}

bool Ingredient::containsAllergen(const std::string& allergen) const {
    return std::any_of(allergens_.begin(), allergens_.end(),
                       [&allergen](const std::string& current) {
                           return equalsIgnoreCase(current, allergen);
                       });
}

std::string Ingredient::unitLabel() const {
    switch (unit_) {
        case Unit::Gram:
            return "g";
        case Unit::Milliliter:
            return "ml";
        case Unit::Piece:
            return "pcs";
    }
    return "";
}
