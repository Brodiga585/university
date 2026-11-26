#ifndef DINE_IN_ORDER_HPP
#define DINE_IN_ORDER_HPP

#include "ordering/Order.hpp"

class Table;
class Waiter;

class DineInOrder : public Order {
public:
    DineInOrder(std::string customerName,
                Table* table,
                int guests,
                double serviceChargeRate = 0.1,
                Waiter* waiter = nullptr);

    Table* table() const;
    Waiter* waiter() const;
    int guests() const;
    double serviceChargeRate() const;

    void setTable(Table* table);
    void setWaiter(Waiter* waiter);
    void setGuests(int guests);

    Money extraFee() const override;
    std::string orderType() const override;
    std::string describe() const override;

private:
    Table* table_;
    Waiter* waiter_;
    int guests_;
    double serviceChargeRate_;
};

#endif
