#include "catalog/MenuCategory.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

MenuCategory::MenuCategory(std::string title, int sortOrder)
    : title_(std::move(title)), sortOrder_(sortOrder) {
    if (title_.empty()) {
        throw std::invalid_argument("Menu category title must not be empty");
    }
}

MenuCategory::MenuCategory(const MenuCategory& other)
    : title_(other.title_), sortOrder_(other.sortOrder_) {
    items_.reserve(other.items_.size());
    for (const auto& item : other.items_) {
        items_.push_back(item->clone());
    }
}

MenuCategory& MenuCategory::operator=(const MenuCategory& other) {
    if (this != &other) {
        MenuCategory copy(other);
        swap(copy);
    }
    return *this;
}

const std::string& MenuCategory::title() const {
    return title_;
}

int MenuCategory::sortOrder() const {
    return sortOrder_;
}

const std::vector<std::unique_ptr<MenuItem>>& MenuCategory::items() const {
    return items_;
}

bool MenuCategory::empty() const {
    return items_.empty();
}

std::size_t MenuCategory::itemCount() const {
    return items_.size();
}

MenuItem& MenuCategory::addItem(std::unique_ptr<MenuItem> item) {
    if (!item) {
        throw std::invalid_argument("Menu item must not be null");
    }
    if (findItem(item->name()) != nullptr) {
        throw std::invalid_argument("Duplicate menu item: " + item->name());
    }
    items_.push_back(std::move(item));
    return *items_.back();
}

bool MenuCategory::removeItem(const std::string& name) {
    const auto position = std::find_if(
        items_.begin(), items_.end(),
        [&name](const std::unique_ptr<MenuItem>& item) {
            return item->name() == name;
        });
    if (position == items_.end()) {
        return false;
    }
    items_.erase(position);
    return true;
}

MenuItem* MenuCategory::findItem(const std::string& name) {
    for (const auto& item : items_) {
        if (item->name() == name) {
            return item.get();
        }
    }
    return nullptr;
}

const MenuItem* MenuCategory::findItem(const std::string& name) const {
    for (const auto& item : items_) {
        if (item->name() == name) {
            return item.get();
        }
    }
    return nullptr;
}

std::vector<const MenuItem*> MenuCategory::availableItems() const {
    std::vector<const MenuItem*> result;
    for (const auto& item : items_) {
        if (item->isAvailable()) {
            result.push_back(item.get());
        }
    }
    return result;
}

void MenuCategory::sortByPrice() {
    std::stable_sort(
        items_.begin(), items_.end(),
        [](const std::unique_ptr<MenuItem>& left, const std::unique_ptr<MenuItem>& right) {
            return left->calculatePrice() < right->calculatePrice();
        });
}

void MenuCategory::swap(MenuCategory& other) noexcept {
    using std::swap;
    swap(title_, other.title_);
    swap(items_, other.items_);
    swap(sortOrder_, other.sortOrder_);
}
