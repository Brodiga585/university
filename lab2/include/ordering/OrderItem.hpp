#ifndef ORDER_ITEM_HPP
#define ORDER_ITEM_HPP

#include "core/Money.hpp"

#include <string>

class MenuItem;

class OrderItem {
public:
    OrderItem(const MenuItem& menuItem, int quantity, std::string comment = {});

    const MenuItem* menuItem() const;
    const std::string& name() const;
    const Money& unitPrice() const;
    int quantity() const;
    const std::string& comment() const;

    void setQuantity(int quantity);
    void setComment(std::string comment);
    Money lineTotal() const;
    std::string describe() const;

private:
    const MenuItem* menuItem_;
    std::string name_;
    Money unitPrice_;
    int quantity_;
    std::string comment_;
};

#endif
