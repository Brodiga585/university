#ifndef MENU_HPP
#define MENU_HPP

#include "catalog/MenuCategory.hpp"
#include "core/Money.hpp"

#include <cstddef>
#include <string>
#include <vector>

class Menu {
public:
    Menu() = default;

    MenuCategory& addCategory(MenuCategory category);
    bool removeCategory(const std::string& title);
    MenuCategory* findCategory(const std::string& title);
    const MenuCategory* findCategory(const std::string& title) const;
    MenuItem* findItem(const std::string& name);
    const MenuItem* findItem(const std::string& name) const;
    std::vector<const MenuItem*> availableItems() const;
    std::vector<const MenuItem*> itemsCheaperThan(const Money& maxPrice) const;
    void publish(const std::string& version);

    const std::vector<MenuCategory>& categories() const;
    const std::string& version() const;
    const std::string& updatedAt() const;
    std::size_t itemCount() const;
    bool empty() const;

private:
    std::vector<MenuCategory> categories_;
    std::string version_;
    std::string updatedAt_;
};

#endif
