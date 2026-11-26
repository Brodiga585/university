#ifndef MENU_CATEGORY_HPP
#define MENU_CATEGORY_HPP

#include "catalog/MenuItem.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

class MenuCategory {
public:
    explicit MenuCategory(std::string title, int sortOrder = 0);
    MenuCategory(const MenuCategory& other);
    MenuCategory& operator=(const MenuCategory& other);
    MenuCategory(MenuCategory&&) noexcept = default;
    MenuCategory& operator=(MenuCategory&&) noexcept = default;
    ~MenuCategory() = default;

    const std::string& title() const;
    int sortOrder() const;
    const std::vector<std::unique_ptr<MenuItem>>& items() const;
    bool empty() const;
    std::size_t itemCount() const;

    MenuItem& addItem(std::unique_ptr<MenuItem> item);
    bool removeItem(const std::string& name);
    MenuItem* findItem(const std::string& name);
    const MenuItem* findItem(const std::string& name) const;
    std::vector<const MenuItem*> availableItems() const;
    void sortByPrice();
    void swap(MenuCategory& other) noexcept;

private:
    std::string title_;
    std::vector<std::unique_ptr<MenuItem>> items_;
    int sortOrder_;
};

#endif
