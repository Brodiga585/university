#include "cli/MainMenu.hpp"

#include <exception>
#include <string>
#include <vector>

namespace {
constexpr int kExitOption = 0;
constexpr int kLastOption = 5;
constexpr double kMinimumPrice = 0.0;
constexpr double kMaximumPrice = 100000.0;
}

MainMenu::MainMenu(ConsoleUI& ui, Menu& menu) : ui_(ui), menu_(menu) {}

void MainMenu::run() {
    bool running = true;
    while (running) {
        ui_.printHeader("Pizzeria Catalog");
        printOptions();
        const int choice = ui_.readInt("Choose an option: ", kExitOption, kLastOption);
        try {
            switch (choice) {
                case 1:
                    showFullMenu();
                    break;
                case 2:
                    showAvailableItems();
                    break;
                case 3:
                    findItem();
                    break;
                case 4:
                    showPizzas();
                    break;
                case 5:
                    showItemsCheaperThan();
                    break;
                default:
                    running = false;
                    break;
            }
        } catch (const std::exception& error) {
            ui_.printLine(std::string("Error: ") + error.what());
        }
        if (running) {
            ui_.waitForEnter();
        }
    }
    ui_.printLine("Goodbye!");
}

void MainMenu::printOptions() const {
    ui_.printLine("1. Show full menu");
    ui_.printLine("2. Show available items");
    ui_.printLine("3. Find item by name");
    ui_.printLine("4. Show pizzas with details");
    ui_.printLine("5. Show items cheaper than a price");
    ui_.printLine("0. Exit");
}

void MainMenu::showFullMenu() const {
    if (menu_.empty()) {
        ui_.printLine("Menu is empty.");
        return;
    }
    ui_.printLine("Version: " + menu_.version() + " (updated " + menu_.updatedAt() + ")");
    for (const auto& category : menu_.categories()) {
        ui_.printBlankLine();
        ui_.printLine("[" + category.title() + "]");
        for (const auto& item : category.items()) {
            printItem(*item);
        }
    }
}

void MainMenu::showAvailableItems() const {
    const auto items = menu_.availableItems();
    if (items.empty()) {
        ui_.printLine("No available items.");
        return;
    }
    for (const MenuItem* item : items) {
        printItem(*item);
    }
}

void MainMenu::findItem() const {
    const std::string name = ui_.readLine("Enter item name: ");
    const MenuItem* item = menu_.findItem(name);
    if (item == nullptr) {
        ui_.printLine("Item not found: " + name);
        return;
    }
    printItem(*item);
    if (const auto* pizza = dynamic_cast<const Pizza*>(item)) {
        printPizzaDetails(*pizza);
    }
}

void MainMenu::showPizzas() const {
    const auto pizzaItems = pizzas();
    if (pizzaItems.empty()) {
        ui_.printLine("No pizzas in the menu.");
        return;
    }
    for (const Pizza* pizza : pizzaItems) {
        ui_.printLine(pizza->name() + " - " + pizza->calculatePrice().toString());
        printPizzaDetails(*pizza);
        ui_.printBlankLine();
    }
}

void MainMenu::showItemsCheaperThan() const {
    const double maximumRubles = ui_.readDouble(
        "Maximum price (RUB): ", kMinimumPrice, kMaximumPrice);
    const Money maxPrice = Money::fromRubles(maximumRubles);
    const auto items = menu_.itemsCheaperThan(maxPrice);
    if (items.empty()) {
        ui_.printLine("No items found below this price.");
        return;
    }
    for (const MenuItem* item : items) {
        printItem(*item);
    }
}

void MainMenu::printItem(const MenuItem& item) const {
    ui_.printLine("- " + item.describe());
}

void MainMenu::printPizzaDetails(const Pizza& pizza) const {
    ui_.printLine("  Size: " + pizza.size().label());
    ui_.printLine("  Dough: " + pizza.dough().label());
    ui_.printLine("  Sauce: " + pizza.sauce().label());

    std::string toppings;
    for (const auto& topping : pizza.toppings()) {
        if (!toppings.empty()) {
            toppings += ", ";
        }
        toppings += topping.describe();
    }
    ui_.printLine("  Toppings: " + (toppings.empty() ? std::string("none") : toppings));
    ui_.printLine("  Calories: " +
                  std::to_string(static_cast<int>(pizza.calculateCalories())));
    ui_.printLine("  Food cost: " + pizza.foodCost().toString());
}

std::vector<const Pizza*> MainMenu::pizzas() const {
    std::vector<const Pizza*> result;
    for (const auto& category : menu_.categories()) {
        for (const auto& item : category.items()) {
            if (const auto* pizza = dynamic_cast<const Pizza*>(item.get())) {
                result.push_back(pizza);
            }
        }
    }
    return result;
}
