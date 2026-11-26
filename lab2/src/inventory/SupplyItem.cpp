#include "inventory/SupplyItem.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <utility>

SupplyItem::SupplyItem(Ingredient ingredient, double quantity, const Money& unitPrice)
    : ingredient_(std::move(ingredient)), quantity_(quantity), unitPrice_(unitPrice) {
    if (!std::isfinite(quantity_) || quantity_ <= 0.0) {
        throw std::invalid_argument("Supply quantity must be finite and positive");
    }
    if (unitPrice_.minorUnits() < 0) {
        throw std::invalid_argument("Supply unit price must not be negative");
    }
}

const Ingredient& SupplyItem::ingredient() const {
    return ingredient_;
}

const std::string& SupplyItem::name() const {
    return ingredient_.name();
}

double SupplyItem::quantity() const {
    return quantity_;
}

const Money& SupplyItem::unitPrice() const {
    return unitPrice_;
}

Money SupplyItem::total() const {
    return unitPrice_ * quantity_;
}

std::string SupplyItem::describe() const {
    std::ostringstream output;
    output << name() << ": " << quantity_ << ' ' << ingredient_.unitLabel()
           << " x " << unitPrice_.toString() << " = " << total().toString();
    return output.str();
}
