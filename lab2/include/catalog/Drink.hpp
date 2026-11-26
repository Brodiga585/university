#ifndef DRINK_HPP
#define DRINK_HPP

#include "catalog/MenuItem.hpp"

#include <memory>
#include <string>

class Drink : public MenuItem {
public:
    Drink(std::string name,
          std::string description,
          const Money& basePrice,
          int volumeMl,
          std::string brand,
          bool alcoholic,
          double caloriesPer100Ml,
          bool available = true);

    int volumeMl() const;
    const std::string& brand() const;
    bool isAlcoholic() const;

    Money calculatePrice() const override;
    double calculateCalories() const override;
    std::string category() const override;
    std::unique_ptr<MenuItem> clone() const override;
    std::string describe() const override;

private:
    int volumeMl_;
    std::string brand_;
    bool alcoholic_;
    double caloriesPer100Ml_;
};

#endif
