#include "ordering/OrderItem.hpp"

#include "catalog/MenuItem.hpp"
#include "exceptions/MenuItemUnavailableException.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

OrderItem::OrderItem(const MenuItem& menuItem, int quantity, std::string comment)
    : menuItem_(&menuItem),
      name_(menuItem.name()),
      unitPrice_(menuItem.calculatePrice()),
      quantity_(quantity),
      comment_(std::move(comment)) {
    if (!menuItem.isAvailable()) {
        throw MenuItemUnavailableException("Menu item is unavailable: " + name_);
    }
    if (quantity_ <= 0) {
        throw std::invalid_argument("Order item quantity must be positive");
    }
}

const MenuItem* OrderItem::menuItem() const {
    return menuItem_;
}

const std::string& OrderItem::name() const {
    return name_;
}

const Money& OrderItem::unitPrice() const {
    return unitPrice_;
}

int OrderItem::quantity() const {
    return quantity_;
}

const std::string& OrderItem::comment() const {
    return comment_;
}

void OrderItem::setQuantity(int quantity) {
    if (quantity <= 0) {
        throw std::invalid_argument("Order item quantity must be positive");
    }
    quantity_ = quantity;
}

void OrderItem::setComment(std::string comment) {
    comment_ = std::move(comment);
}

Money OrderItem::lineTotal() const {
    return unitPrice_ * quantity_;
}

std::string OrderItem::describe() const {
    std::ostringstream output;
    output << name_ << " x" << quantity_ << " = " << lineTotal().toString();
    if (!comment_.empty()) {
        output << " (" << comment_ << ')';
    }
    return output.str();
}
