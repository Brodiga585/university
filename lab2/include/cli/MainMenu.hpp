#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "catalog/Menu.hpp"
#include "catalog/Pizza.hpp"
#include "cli/ConsoleUI.hpp"

#include <vector>

class MainMenu {
public:
    MainMenu(ConsoleUI& ui, Menu& menu);
    void run();

private:
    void printOptions() const;
    void showFullMenu() const;
    void showAvailableItems() const;
    void findItem() const;
    void showPizzas() const;
    void showItemsCheaperThan() const;
    void printItem(const MenuItem& item) const;
    void printPizzaDetails(const Pizza& pizza) const;
    std::vector<const Pizza*> pizzas() const;

    ConsoleUI& ui_;
    Menu& menu_;
};

#endif
