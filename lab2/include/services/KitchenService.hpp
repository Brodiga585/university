#ifndef KITCHEN_SERVICE_HPP
#define KITCHEN_SERVICE_HPP

#include "core/Entity.hpp"
#include "ordering/KitchenQueue.hpp"

#include <cstddef>
#include <vector>

class Chef;
class Order;

class KitchenService {
public:
    KitchenService() = default;

    KitchenTicket* acceptOrder(Order& order,
                               Chef* chef = nullptr,
                               int priority = 0,
                               int estimatedMinutes = 15);
    bool startNext();
    bool markNextReady();
    bool serveNextReady();

    KitchenTicket* findForOrder(Entity::Id orderId);
    const KitchenTicket* findForOrder(Entity::Id orderId) const;
    std::vector<const KitchenTicket*> pendingTickets() const;
    std::size_t queuedCount() const;
    std::size_t size() const;

    KitchenQueue& queue();
    const KitchenQueue& queue() const;

private:
    KitchenQueue queue_;
};

#endif
