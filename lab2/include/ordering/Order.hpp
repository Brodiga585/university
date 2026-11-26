#ifndef ORDER_HPP
#define ORDER_HPP

#include "core/Entity.hpp"
#include "core/Money.hpp"
#include "ordering/OrderItem.hpp"
#include "ordering/OrderStatus.hpp"

#include <cstddef>
#include <ctime>
#include <memory>
#include <string>
#include <vector>

class MenuItem;
class Payment;
class PromoCode;

class Order : public Entity {
public:
    explicit Order(std::string customerName);
    ~Order() override;

    const std::string& customerName() const;
    OrderStatus status() const;
    const std::vector<OrderItem>& items() const;
    std::size_t itemCount() const;
    const std::string& comment() const;
    void setComment(std::string comment);
    const Money& discount() const;
    const std::string& promoCode() const;

    OrderItem& addItem(const MenuItem& menuItem, int quantity, std::string comment = {});
    void addItem(const OrderItem& item);
    bool removeItem(const std::string& name);
    void clearItems();

    void applyPromoCode(const PromoCode& promoCode, std::time_t now);
    void clearPromoCode();

    Money subtotal() const;
    Money discountedSubtotal() const;
    virtual Money extraFee() const = 0;
    Money total() const;

    void setPayment(std::unique_ptr<Payment> payment);
    const Payment* payment() const;
    bool hasPayment() const;
    bool isPaid() const;

    void checkout();
    void place();
    void confirm();
    void startPreparing();
    void markReady();
    virtual void dispatch();
    virtual void complete();
    void cancel();

    virtual std::string orderType() const = 0;
    virtual std::string describe() const;

protected:
    void ensureStatus(OrderStatus expected, const std::string& operation) const;
    void ensureDraft(const std::string& operation) const;

private:
    std::string customerName_;
    std::vector<OrderItem> items_;
    OrderStatus status_;
    std::string comment_;
    Money discount_;
    std::string promoCode_;
    std::unique_ptr<Payment> payment_;
};

#endif
