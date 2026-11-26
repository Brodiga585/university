#include "catalog/Recipe.hpp"

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <utility>

Recipe::Recipe(std::string title, int cookingMinutes)
    : title_(std::move(title)), cookingMinutes_(cookingMinutes) {
    if (title_.empty()) {
        throw std::invalid_argument("Recipe title must not be empty");
    }
    if (cookingMinutes_ <= 0) {
        throw std::invalid_argument("Recipe cooking time must be positive");
    }
}

const std::string& Recipe::title() const {
    return title_;
}

int Recipe::cookingMinutes() const {
    return cookingMinutes_;
}

const std::vector<std::string>& Recipe::steps() const {
    return steps_;
}

const std::vector<Recipe::IngredientAmount>& Recipe::ingredients() const {
    return ingredients_;
}

void Recipe::addStep(std::string step) {
    if (step.empty()) {
        throw std::invalid_argument("Recipe step must not be empty");
    }
    steps_.push_back(std::move(step));
}

void Recipe::addIngredient(const Ingredient& ingredient, double amount) {
    if (!std::isfinite(amount) || amount <= 0.0) {
        throw std::invalid_argument("Recipe ingredient amount must be finite and positive");
    }
    ingredients_.push_back({ingredient, amount});
}

Money Recipe::totalCost() const {
    if (ingredients_.empty()) {
        return Money();
    }
    Money total = ingredients_.front().ingredient.costFor(ingredients_.front().amount);
    for (std::size_t index = 1; index < ingredients_.size(); ++index) {
        total += ingredients_[index].ingredient.costFor(ingredients_[index].amount);
    }
    return total;
}

double Recipe::totalCalories() const {
    double total = 0.0;
    for (const auto& item : ingredients_) {
        total += item.ingredient.caloriesFor(item.amount);
    }
    return total;
}

void Recipe::validate() const {
    if (steps_.empty()) {
        throw std::runtime_error("Recipe has no steps: " + title_);
    }
    if (ingredients_.empty()) {
        throw std::runtime_error("Recipe has no ingredients: " + title_);
    }
}
