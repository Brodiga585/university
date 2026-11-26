#ifndef INGREDIENT_HPP
#define INGREDIENT_HPP

#include "core/Money.hpp"

#include <string>
#include <vector>

class Ingredient {
public:
    enum class Unit {
        Gram,
        Milliliter,
        Piece
    };

    Ingredient(std::string name,
               Unit unit,
               const Money& costPerUnit,
               double caloriesPerUnit,
               std::vector<std::string> allergens = {},
               bool vegetarian = true);

    const std::string& name() const;
    Unit unit() const;
    const Money& costPerUnit() const;
    double caloriesPerUnit() const;
    bool isVegetarian() const;

    Money costFor(double amount) const;
    double caloriesFor(double amount) const;
    bool containsAllergen(const std::string& allergen) const;
    std::string unitLabel() const;

private:
    std::string name_;
    Unit unit_;
    Money costPerUnit_;
    double caloriesPerUnit_;
    std::vector<std::string> allergens_;
    bool vegetarian_;
};

#endif
