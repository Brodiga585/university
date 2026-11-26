#include "ordering/Order.hpp"

#include "exceptions/EmptyOrderException.hpp"
#include "exceptions/InvalidOrderException.hpp"
#include "finance/Payment.hpp"
#include "finance/PromoCode.hpp"

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <utility>

Order::Order(std::string customerName)
    : customerName_(std::move(customerName)), status_(OrderStatus::Draft) {
    if (customerName_.empty()) {
        throw std::invalid_argument("Order customer name must not be empty");
    }
}

Order::~Order() = default;

const std::string& Order::customerName() const {
    return customerName_;
}

OrderStatus Order::status() const {
    return status_;
}

const std::vector<OrderItem>& Order::items() const {
    return items_;
}

std::size_t Order::itemCount() const {
    return items_.size();
}

const std::string& Order::comment() const {
    return comment_;
}

void Order::setComment(std::string comment) {
    comment_ = std::move(comment);
}

const Money& Order::discount() const {
    return discount_;
}

const std::string& Order::promoCode() const {
    return promoCode_;
}

OrderItem& Order::addItem(const MenuItem& menuItem, int quantity, std::string comment) {
    ensureDraft("add items to");
    const auto existing = std::find_if(
        items_.begin(), items_.end(),
        [&menuItem](const OrderItem& item) {
            return item.menuItem() == &menuItem && item.comment().empty();
        });
    if (existing != items_.end()) {
        existing->setQuantity(existing->quantity() + quantity);
        return *existing;
    }
    items_.emplace_back(menuItem, quantity, std::move(comment));
    return items_.back();
}

void Order::addItem(const OrderItem& item) {
    ensureDraft("add items to");
    items_.push_back(item);
}

bool Order::removeItem(const std::string& name) {
    ensureDraft("remove items from");
    const auto position = std::find_if(
        items_.begin(), items_.end(),
        [&name](const OrderItem& item) {
            return item.name() == name;
        });
    if (position == items_.end()) {
        return false;
    }
    items_.erase(position);
    return true;
}

void Order::clearItems() {
    ensureDraft("clear items of");
    items_.clear();
}

void Order::applyPromoCode(const PromoCode& promoCode, std::time_t now) {
    ensureDraft("apply a promo code to");
    if (items_.empty()) {
        throw EmptyOrderException("Cannot apply a promo code to an empty order");
    }
    discount_ = promoCode.discountFor(subtotal(), now);
    promoCode_ = promoCode.code();
}

void Order::clearPromoCode() {
    ensureDraft("clear the promo code of");
    discount_ = Money();
    promoCode_.clear();
}

Money Order::subtotal() const {
    Money result;
    for (const auto& item : items_) {
        result += item.lineTotal();
    }
    return result;
}

Money Order::discountedSubtotal() const {
    return subtotal() - discount_;
}

Money Order::total() const {
    return discountedSubtotal() + extraFee();
}

void Order::setPayment(std::unique_ptr<Payment> payment) {
    ensureDraft("set payment for");
    if (!payment) {
        throw std::invalid_argument("Payment must not be null");
    }
    payment_ = std::move(payment);
}

const Payment* Order::payment() const {
    return payment_.get();
}

bool Order::hasPayment() const {
    return payment_ != nullptr;
}

bool Order::isPaid() const {
    return payment_ != nullptr && payment_->status() == PaymentStatus::Completed;
}

void Order::checkout() {
    ensureStatus(OrderStatus::Draft, "check out");
    if (items_.empty()) {
        throw EmptyOrderException("Cannot check out an empty order");
    }
    if (!payment_) {
        throw InvalidOrderException("Order has no payment method");
    }
    payment_->pay();
    status_ = OrderStatus::Placed;
}

void Order::place() {
    ensureStatus(OrderStatus::Draft, "place");
    if (items_.empty()) {
        throw EmptyOrderException("Cannot place an empty order");
    }
    status_ = OrderStatus::Placed;
}

void Order::confirm() {
    ensureStatus(OrderStatus::Placed, "confirm");
    status_ = OrderStatus::Confirmed;
}

void Order::startPreparing() {
    ensureStatus(OrderStatus::Confirmed, "start preparing");
    status_ = OrderStatus::Preparing;
}

void Order::markReady() {
    ensureStatus(OrderStatus::Preparing, "mark ready");
    status_ = OrderStatus::Ready;
}

void Order::dispatch() {
    ensureStatus(OrderStatus::Ready, "dispatch");
    status_ = OrderStatus::OutForDelivery;
}

void Order::complete() {
    if (status_ != OrderStatus::Ready && status_ != OrderStatus::OutForDelivery) {
        throw InvalidOrderException("Only ready or dispatched orders can be completed");
    }
    status_ = OrderStatus::Completed;
}

void Order::cancel() {
    if (isFinalStatus(status_)) {
        throw InvalidOrderException("Order is already " + orderStatusLabel(status_));
    }
    if (payment_ && payment_->status() == PaymentStatus::Completed) {
        payment_->refund();
    }
    status_ = OrderStatus::Cancelled;
}

std::string Order::describe() const {
    std::ostringstream output;
    output << orderType() << " order #" << id() << " for " << customerName_
           << " [" << orderStatusLabel(status_) << "] " << itemCount()
           << " items, total " << total().toString();
    return output.str();
}

void Order::ensureStatus(OrderStatus expected, const std::string& operation) const {
    if (status_ != expected) {
        throw InvalidOrderException(
            "Cannot " + operation + " order in status " + orderStatusLabel(status_));
    }
}

void Order::ensureDraft(const std::string& operation) const {
    ensureStatus(OrderStatus::Draft, operation);
}
