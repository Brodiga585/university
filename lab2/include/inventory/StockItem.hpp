#ifndef STOCK_ITEM_HPP
#define STOCK_ITEM_HPP

#include "catalog/Ingredient.hpp"
#include "core/Money.hpp"

#include <ctime>
#include <string>

class StockItem {
public:
    StockItem(Ingredient ingredient,
              double quantity,
              double reserved,
              double minimumQuantity,
              std::time_t expiryDate);

    const Ingredient& ingredient() const;
    const std::string& name() const;
    double quantity() const;
    double reserved() const;
    double available() const;
    double minimumQuantity() const;
    std::time_t expiryDate() const;

    bool isLow() const;
    bool isExpired(std::time_t now) const;
    bool isExpiringSoon(std::time_t now, int days) const;

    void replenish(double amount);
    void reserve(double amount);
    void release(double amount);
    void consume(double amount);
    void writeOff(double amount);
    void setMinimumQuantity(double minimumQuantity);

    Money stockValue() const;
    std::string describe() const;

private:
    void ensureAmount(double amount, const std::string& operation) const;

    Ingredient ingredient_;
    double quantity_;
    double reserved_;
    double minimumQuantity_;
    std::time_t expiryDate_;
};

#endif
