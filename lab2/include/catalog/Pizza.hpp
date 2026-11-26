#ifndef PIZZA_HPP
#define PIZZA_HPP

#include "catalog/Dough.hpp"
#include "catalog/MenuItem.hpp"
#include "catalog/PizzaSize.hpp"
#include "catalog/Recipe.hpp"
#include "catalog/Sauce.hpp"
#include "catalog/Topping.hpp"

#include <memory>
#include <string>
#include <vector>

class Pizza : public MenuItem {
public:
    Pizza(std::string name,
          std::string description,
          const Money& basePrice,
          PizzaSize size,
          Dough dough,
          Sauce sauce,
          Recipe recipe,
          std::vector<Topping> toppings = {});

    const PizzaSize& size() const;
    const Dough& dough() const;
    const Sauce& sauce() const;
    const Recipe& recipe() const;
    const std::vector<Topping>& toppings() const;

    void addTopping(const Topping& topping);
    bool removeTopping(const std::string& name);
    Money foodCost() const;

    Money calculatePrice() const override;
    double calculateCalories() const override;
    std::string category() const override;
    std::unique_ptr<MenuItem> clone() const override;
    std::string describe() const override;

private:
    PizzaSize size_;
    Dough dough_;
    Sauce sauce_;
    std::vector<Topping> toppings_;
    Recipe recipe_;
};

#endif
