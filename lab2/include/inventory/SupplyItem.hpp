#ifndef SUPPLY_ITEM_HPP
#define SUPPLY_ITEM_HPP

#include "catalog/Ingredient.hpp"
#include "core/Money.hpp"

class SupplyItem {
public:
    SupplyItem(Ingredient ingredient, double quantity, const Money& unitPrice);

    const Ingredient& ingredient() const;
    const std::string& name() const;
    double quantity() const;
    const Money& unitPrice() const;

    Money total() const;
    std::string describe() const;

private:
    Ingredient ingredient_;
    double quantity_;
    Money unitPrice_;
};

#endif
