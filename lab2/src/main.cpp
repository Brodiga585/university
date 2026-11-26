#include "catalog/Dough.hpp"
#include "catalog/Drink.hpp"
#include "catalog/Ingredient.hpp"
#include "catalog/Menu.hpp"
#include "catalog/MenuCategory.hpp"
#include "catalog/Pizza.hpp"
#include "catalog/PizzaSize.hpp"
#include "catalog/Recipe.hpp"
#include "catalog/Sauce.hpp"
#include "catalog/Topping.hpp"
#include "cli/ConsoleUI.hpp"
#include "cli/MainMenu.hpp"
#include "core/Money.hpp"

#include <exception>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

namespace {
Ingredient makeFlour() {
    return Ingredient("Flour", Ingredient::Unit::Gram, Money::fromRubles(0.08), 3.6, {"Gluten"});
}

Ingredient makeTomato() {
    return Ingredient("Tomato", Ingredient::Unit::Gram, Money::fromRubles(0.15), 0.2);
}

Ingredient makeMozzarella() {
    return Ingredient("Mozzarella", Ingredient::Unit::Gram, Money::fromRubles(0.6), 2.5, {"Milk"});
}

Ingredient makeBasil() {
    return Ingredient("Basil", Ingredient::Unit::Gram, Money::fromRubles(1.0), 0.4);
}

Ingredient makePepperoni() {
    return Ingredient("Pepperoni", Ingredient::Unit::Gram, Money::fromRubles(0.9), 4.0, {}, false);
}

Recipe makeMargheritaRecipe(const Ingredient& flour, const Ingredient& tomato) {
    Recipe recipe("Margherita recipe", 12);
    recipe.addIngredient(flour, 180.0);
    recipe.addIngredient(tomato, 80.0);
    recipe.addStep("Knead the dough and let it rest");
    recipe.addStep("Spread tomato sauce over the base");
    recipe.addStep("Bake until the crust is golden");
    return recipe;
}

Recipe makePepperoniRecipe(const Ingredient& flour, const Ingredient& tomato) {
    Recipe recipe("Pepperoni recipe", 14);
    recipe.addIngredient(flour, 180.0);
    recipe.addIngredient(tomato, 80.0);
    recipe.addStep("Knead the dough and let it rest");
    recipe.addStep("Spread tomato sauce and add toppings");
    recipe.addStep("Bake until the cheese melts");
    return recipe;
}

Menu buildDemoMenu() {
    const Ingredient flour = makeFlour();
    const Ingredient tomato = makeTomato();
    const Ingredient mozzarella = makeMozzarella();
    const Ingredient basil = makeBasil();
    const Ingredient pepperoni = makePepperoni();

    const Dough classicDough(Dough::Type::Classic, "Classic", 1.0);
    const Dough thinDough(Dough::Type::Thin, "Thin", 1.0);
    const Sauce tomatoSauce("Tomato", 0, 1.0, true);
    const Sauce spicySauce("Spicy", 3, 1.1, true);
    const PizzaSize smallSize("Small", 25.0, 0.8, 6);
    const PizzaSize mediumSize("Medium", 30.0, 1.0, 8);
    const PizzaSize largeSize("Large", 35.0, 1.25, 10);

    const Topping mozzarellaTopping(mozzarella, 100.0, Money::fromRubles(60.0));
    const Topping basilTopping(basil, 10.0, Money::fromRubles(30.0));
    const Topping pepperoniTopping(pepperoni, 60.0, Money::fromRubles(90.0));

    auto margherita = std::make_unique<Pizza>(
        "Margherita",
        "Tomato sauce, mozzarella and basil",
        Money::fromRubles(350.0),
        mediumSize,
        classicDough,
        tomatoSauce,
        makeMargheritaRecipe(flour, tomato),
        std::vector<Topping>{mozzarellaTopping, basilTopping});

    auto pepperoniPizza = std::make_unique<Pizza>(
        "Pepperoni",
        "Tomato sauce, mozzarella and pepperoni",
        Money::fromRubles(450.0),
        largeSize,
        thinDough,
        spicySauce,
        makePepperoniRecipe(flour, tomato),
        std::vector<Topping>{mozzarellaTopping, pepperoniTopping});

    auto cola = std::make_unique<Drink>(
        "Cola", "Chilled cola", Money::fromRubles(120.0), 500, "ColaCo", false, 42.0);
    auto lemonade = std::make_unique<Drink>(
        "Lemonade", "House lemonade", Money::fromRubles(150.0), 400, "Pizzeria", false, 25.0);

    MenuCategory pizzas("Pizzas", 1);
    pizzas.addItem(std::move(margherita));
    pizzas.addItem(std::move(pepperoniPizza));

    MenuCategory drinks("Drinks", 2);
    drinks.addItem(std::move(cola));
    drinks.addItem(std::move(lemonade));

    Menu menu;
    menu.addCategory(std::move(pizzas));
    menu.addCategory(std::move(drinks));
    menu.publish("1.0");
    return menu;
}
}

int main() {
    try {
        Menu menu = buildDemoMenu();
        ConsoleUI ui;
        MainMenu application(ui, menu);
        application.run();
    } catch (const std::exception& error) {
        std::cerr << "Fatal error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
