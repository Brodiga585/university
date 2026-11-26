#include "catalog/Menu.hpp"

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <utility>

MenuCategory& Menu::addCategory(MenuCategory category) {
    if (findCategory(category.title()) != nullptr) {
        throw std::invalid_argument("Duplicate menu category: " + category.title());
    }
    categories_.push_back(std::move(category));
    return categories_.back();
}

bool Menu::removeCategory(const std::string& title) {
    const auto position = std::find_if(
        categories_.begin(), categories_.end(),
        [&title](const MenuCategory& category) {
            return category.title() == title;
        });
    if (position == categories_.end()) {
        return false;
    }
    categories_.erase(position);
    return true;
}

MenuCategory* Menu::findCategory(const std::string& title) {
    for (auto& category : categories_) {
        if (category.title() == title) {
            return &category;
        }
    }
    return nullptr;
}

const MenuCategory* Menu::findCategory(const std::string& title) const {
    for (const auto& category : categories_) {
        if (category.title() == title) {
            return &category;
        }
    }
    return nullptr;
}

MenuItem* Menu::findItem(const std::string& name) {
    for (auto& category : categories_) {
        MenuItem* item = category.findItem(name);
        if (item != nullptr) {
            return item;
        }
    }
    return nullptr;
}

const MenuItem* Menu::findItem(const std::string& name) const {
    for (const auto& category : categories_) {
        const MenuItem* item = category.findItem(name);
        if (item != nullptr) {
            return item;
        }
    }
    return nullptr;
}

std::vector<const MenuItem*> Menu::availableItems() const {
    std::vector<const MenuItem*> result;
    for (const auto& category : categories_) {
        const auto categoryItems = category.availableItems();
        result.insert(result.end(), categoryItems.begin(), categoryItems.end());
    }
    return result;
}

std::vector<const MenuItem*> Menu::itemsCheaperThan(const Money& maxPrice) const {
    std::vector<const MenuItem*> result;
    for (const auto& category : categories_) {
        for (const auto& item : category.items()) {
            if (item->calculatePrice() < maxPrice) {
                result.push_back(item.get());
            }
        }
    }
    return result;
}

void Menu::publish(const std::string& version) {
    if (version.empty()) {
        throw std::invalid_argument("Menu version must not be empty");
    }
    version_ = version;

    const std::time_t now = std::time(nullptr);
    const std::tm* localTime = std::localtime(&now);
    if (localTime == nullptr) {
        throw std::runtime_error("Failed to read local time");
    }
    std::ostringstream output;
    output << std::put_time(localTime, "%Y-%m-%d %H:%M");
    updatedAt_ = output.str();
}

const std::vector<MenuCategory>& Menu::categories() const {
    return categories_;
}

const std::string& Menu::version() const {
    return version_;
}

const std::string& Menu::updatedAt() const {
    return updatedAt_;
}

std::size_t Menu::itemCount() const {
    std::size_t total = 0;
    for (const auto& category : categories_) {
        total += category.itemCount();
    }
    return total;
}

bool Menu::empty() const {
    return categories_.empty();
}
