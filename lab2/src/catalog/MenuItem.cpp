#include "catalog/MenuItem.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

MenuItem::MenuItem(std::string name, std::string description, const Money& basePrice, bool available)
    : name_(std::move(name)),
      description_(std::move(description)),
      basePrice_(basePrice),
      available_(available) {
    if (name_.empty()) {
        throw std::invalid_argument("Menu item name must not be empty");
    }
    if (basePrice_.minorUnits() < 0) {
        throw std::invalid_argument("Menu item price must not be negative");
    }
}

const std::string& MenuItem::name() const {
    return name_;
}

const std::string& MenuItem::description() const {
    return description_;
}

const Money& MenuItem::basePrice() const {
    return basePrice_;
}

bool MenuItem::isAvailable() const {
    return available_;
}

void MenuItem::setAvailable(bool available) {
    available_ = available;
}

std::string MenuItem::describe() const {
    std::ostringstream output;
    output << name_ << " - " << description_ << " [" << calculatePrice().toString() << ']';
    if (!available_) {
        output << " (unavailable)";
    }
    return output.str();
}

void MenuItem::ensureAvailable() const {
    if (!available_) {
        throw std::runtime_error("Menu item is unavailable: " + name_);
    }
}
