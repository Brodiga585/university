#ifndef KITCHEN_TICKET_HPP
#define KITCHEN_TICKET_HPP

#include <ctime>
#include <string>

class Chef;
class Order;

class KitchenTicket {
public:
    enum class Status {
        Queued,
        Cooking,
        Ready,
        Served
    };

    KitchenTicket(Order* order, Chef* chef, int priority, int estimatedMinutes);

    Order* order() const;
    Chef* chef() const;
    int priority() const;
    int estimatedMinutes() const;
    Status status() const;
    bool isActive() const;

    std::time_t createdAt() const;
    std::time_t startedAt() const;
    std::time_t completedAt() const;

    void start();
    void markReady();
    void serve();

    bool isOverdue(std::time_t now) const;
    std::string statusLabel() const;
    std::string describe() const;

private:
    Order* order_;
    Chef* chef_;
    int priority_;
    int estimatedMinutes_;
    Status status_;
    std::time_t createdAt_;
    std::time_t startedAt_;
    std::time_t completedAt_;
};

#endif
