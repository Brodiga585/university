#ifndef KITCHEN_QUEUE_HPP
#define KITCHEN_QUEUE_HPP

#include "ordering/KitchenTicket.hpp"

#include <cstddef>
#include <vector>

class KitchenQueue {
public:
    KitchenQueue() = default;

    void enqueue(const KitchenTicket& ticket);
    KitchenTicket* next();
    const KitchenTicket* next() const;
    bool startNext();
    bool serveNextReady();

    std::size_t size() const;
    std::size_t queuedCount() const;
    bool empty() const;

    std::vector<KitchenTicket>& tickets();
    const std::vector<KitchenTicket>& tickets() const;

private:
    std::vector<KitchenTicket> tickets_;
};

#endif
