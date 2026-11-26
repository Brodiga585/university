#ifndef RECIPE_HPP
#define RECIPE_HPP

#include "catalog/Ingredient.hpp"
#include "core/Money.hpp"

#include <string>
#include <vector>

class Recipe {
public:
    struct IngredientAmount {
        Ingredient ingredient;
        double amount;
    };

    Recipe(std::string title, int cookingMinutes);

    const std::string& title() const;
    int cookingMinutes() const;
    const std::vector<std::string>& steps() const;
    const std::vector<IngredientAmount>& ingredients() const;

    void addStep(std::string step);
    void addIngredient(const Ingredient& ingredient, double amount);
    Money totalCost() const;
    double totalCalories() const;
    void validate() const;

private:
    std::string title_;
    std::vector<std::string> steps_;
    std::vector<IngredientAmount> ingredients_;
    int cookingMinutes_;
};

#endif
