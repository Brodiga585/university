#include "finance/Receipt.hpp"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
constexpr double kMinimumTaxRate = 0.0;
constexpr double kMaximumTaxRate = 1.0;
}

Money Receipt::Line::lineTotal() const {
    return unitPrice * quantity;
}

std::string Receipt::Line::describe() const {
    std::ostringstream output;
    output << std::left << std::setw(30) << description
           << std::right << std::setw(3) << quantity
           << std::setw(14) << lineTotal().toString();
    return output.str();
}

Receipt::Receipt(std::string orderNumber, double taxRate)
    : orderNumber_(std::move(orderNumber)), taxRate_(taxRate) {
    if (orderNumber_.empty()) {
        throw std::invalid_argument("Receipt order number must not be empty");
    }
    if (!std::isfinite(taxRate_) || taxRate_ < kMinimumTaxRate || taxRate_ > kMaximumTaxRate) {
        throw std::invalid_argument("Receipt tax rate must be between 0 and 1");
    }
}

void Receipt::addLine(std::string description, int quantity, const Money& unitPrice) {
    if (description.empty()) {
        throw std::invalid_argument("Receipt line description must not be empty");
    }
    if (quantity <= 0) {
        throw std::invalid_argument("Receipt line quantity must be positive");
    }
    if (unitPrice.minorUnits() < 0) {
        throw std::invalid_argument("Receipt line price must not be negative");
    }
    lines_.push_back({std::move(description), quantity, unitPrice});
}

void Receipt::setDiscount(const Money& discount) {
    if (discount.minorUnits() < 0) {
        throw std::invalid_argument("Receipt discount must not be negative");
    }
    if (subtotal() < discount) {
        throw std::invalid_argument("Receipt discount cannot exceed subtotal");
    }
    discount_ = discount;
}

void Receipt::setTip(const Money& tip) {
    if (tip.minorUnits() < 0) {
        throw std::invalid_argument("Receipt tip must not be negative");
    }
    tip_ = tip;
}

void Receipt::setPaymentMethod(std::string paymentMethod) {
    if (paymentMethod.empty()) {
        throw std::invalid_argument("Receipt payment method must not be empty");
    }
    paymentMethod_ = std::move(paymentMethod);
}

const std::string& Receipt::orderNumber() const {
    return orderNumber_;
}

const std::vector<Receipt::Line>& Receipt::lines() const {
    return lines_;
}

const Money& Receipt::discount() const {
    return discount_;
}

const Money& Receipt::tip() const {
    return tip_;
}

double Receipt::taxRate() const {
    return taxRate_;
}

const std::string& Receipt::paymentMethod() const {
    return paymentMethod_;
}

Money Receipt::subtotal() const {
    Money result;
    for (const auto& line : lines_) {
        result += line.lineTotal();
    }
    return result;
}

Money Receipt::tax() const {
    return (subtotal() - discount_) * taxRate_;
}

Money Receipt::total() const {
    return subtotal() - discount_ + tax() + tip_;
}

std::string Receipt::format() const {
    std::ostringstream output;
    output << "Receipt for order " << orderNumber_ << '\n';
    output << std::string(47, '-') << '\n';
    for (const auto& line : lines_) {
        output << line.describe() << '\n';
    }
    output << std::string(47, '-') << '\n';
    output << std::left << std::setw(34) << "Subtotal"
           << std::right << std::setw(13) << subtotal().toString() << '\n';
    if (discount_.minorUnits() > 0) {
        output << std::left << std::setw(34) << "Discount"
               << std::right << std::setw(13) << ('-' + discount_.toString()) << '\n';
    }
    output << std::left << std::setw(34) << "Tax"
           << std::right << std::setw(13) << tax().toString() << '\n';
    if (tip_.minorUnits() > 0) {
        output << std::left << std::setw(34) << "Tip"
               << std::right << std::setw(13) << tip_.toString() << '\n';
    }
    output << std::string(47, '-') << '\n';
    output << std::left << std::setw(34) << "Total"
           << std::right << std::setw(13) << total().toString() << '\n';
    if (!paymentMethod_.empty()) {
        output << "Paid by: " << paymentMethod_ << '\n';
    }
    return output.str();
}
