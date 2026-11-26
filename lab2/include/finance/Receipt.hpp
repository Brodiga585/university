#ifndef RECEIPT_HPP
#define RECEIPT_HPP

#include "core/Money.hpp"

#include <string>
#include <vector>

class Receipt {
public:
    struct Line {
        std::string description;
        int quantity;
        Money unitPrice;

        Money lineTotal() const;
        std::string describe() const;
    };

    explicit Receipt(std::string orderNumber, double taxRate = 0.2);

    void addLine(std::string description, int quantity, const Money& unitPrice);
    void setDiscount(const Money& discount);
    void setTip(const Money& tip);
    void setPaymentMethod(std::string paymentMethod);

    const std::string& orderNumber() const;
    const std::vector<Line>& lines() const;
    const Money& discount() const;
    const Money& tip() const;
    double taxRate() const;
    const std::string& paymentMethod() const;

    Money subtotal() const;
    Money tax() const;
    Money total() const;
    std::string format() const;

private:
    std::string orderNumber_;
    std::vector<Line> lines_;
    Money discount_;
    Money tip_;
    double taxRate_;
    std::string paymentMethod_;
};

#endif
