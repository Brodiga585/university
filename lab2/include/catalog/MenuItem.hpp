#ifndef MENU_ITEM_HPP
#define MENU_ITEM_HPP

#include "core/Entity.hpp"
#include "core/Money.hpp"

#include <memory>
#include <string>

class MenuItem : public Entity {
public:
    MenuItem(std::string name, std::string description, const Money& basePrice, bool available = true);
    ~MenuItem() override = default;

    const std::string& name() const;
    const std::string& description() const;
    const Money& basePrice() const;
    bool isAvailable() const;
    void setAvailable(bool available);

    virtual Money calculatePrice() const = 0;
    virtual double calculateCalories() const = 0;
    virtual std::string category() const = 0;
    virtual std::unique_ptr<MenuItem> clone() const = 0;
    virtual std::string describe() const;

protected:
    void ensureAvailable() const;

    std::string name_;
    std::string description_;
    Money basePrice_;
    bool available_;
};

#endif
