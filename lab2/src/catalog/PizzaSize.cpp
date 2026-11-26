#include "catalog/PizzaSize.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
constexpr double kPi = 3.14159265358979323846;
constexpr double kDiameterDivider = 2.0;
}

PizzaSize::PizzaSize(std::string name, double diameterCm, double priceMultiplier, int slices)
    : name_(std::move(name)),
      diameterCm_(diameterCm),
      priceMultiplier_(priceMultiplier),
      slices_(slices) {
    if (name_.empty()) {
        throw std::invalid_argument("Pizza size name must not be empty");
    }
    if (!std::isfinite(diameterCm_) || diameterCm_ <= 0.0) {
        throw std::invalid_argument("Pizza diameter must be finite and positive");
    }
    if (!std::isfinite(priceMultiplier_) || priceMultiplier_ <= 0.0) {
        throw std::invalid_argument("Pizza price multiplier must be finite and positive");
    }
    if (slices_ <= 0) {
        throw std::invalid_argument("Pizza slice count must be positive");
    }
}

const std::string& PizzaSize::name() const {
    return name_;
}

double PizzaSize::diameterCm() const {
    return diameterCm_;
}

double PizzaSize::priceMultiplier() const {
    return priceMultiplier_;
}

int PizzaSize::slices() const {
    return slices_;
}

Money PizzaSize::priceFor(const Money& basePrice) const {
    return basePrice * priceMultiplier_;
}

double PizzaSize::area() const {
    const double radius = diameterCm_ / kDiameterDivider;
    return kPi * radius * radius;
}

double PizzaSize::sliceArea() const {
    return area() / slices_;
}

std::string PizzaSize::label() const {
    std::ostringstream output;
    output << name_ << " (" << diameterCm_ << " cm)";
    return output.str();
}
