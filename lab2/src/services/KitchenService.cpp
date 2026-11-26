#include "services/KitchenService.hpp"

#include "exceptions/InvalidOrderException.hpp"
#include "ordering/Order.hpp"
#include "people/Chef.hpp"

#include <stdexcept>

KitchenTicket* KitchenService::acceptOrder(Order& order,
                                           Chef* chef,
                                           int priority,
                                           int estimatedMinutes) {
    const OrderStatus status = order.status();
    if (status != OrderStatus::Placed && status != OrderStatus::Confirmed) {
        throw InvalidOrderException(
            "Only placed or confirmed orders can be sent to the kitchen, got " +
            orderStatusLabel(status));
    }
    if (findForOrder(order.id()) != nullptr) {
        throw InvalidOrderException("Order already has a kitchen ticket");
    }
    if (chef != nullptr && !chef->canTakeTicket()) {
        throw std::logic_error("Chef cannot take another ticket");
    }
    queue_.enqueue(KitchenTicket(&order, chef, priority, estimatedMinutes));
    return &queue_.tickets().back();
}

bool KitchenService::startNext() {
    KitchenTicket* ticket = queue_.next();
    if (ticket == nullptr) {
        return false;
    }
    if (ticket->chef() != nullptr && !ticket->chef()->canTakeTicket()) {
        throw std::logic_error("Assigned chef cannot take another ticket");
    }
    Order* order = ticket->order();
    if (order->status() == OrderStatus::Placed) {
        order->confirm();
    }
    if (order->status() != OrderStatus::Confirmed &&
        order->status() != OrderStatus::Preparing) {
        throw InvalidOrderException(
            "Cannot start cooking for order in status " +
            orderStatusLabel(order->status()));
    }
    if (order->status() == OrderStatus::Confirmed) {
        order->startPreparing();
    }
    ticket->start();
    if (ticket->chef() != nullptr) {
        ticket->chef()->beginTicket();
    }
    return true;
}

bool KitchenService::markNextReady() {
    for (auto& ticket : queue_.tickets()) {
        if (ticket.status() != KitchenTicket::Status::Cooking) {
            continue;
        }
        Order* order = ticket.order();
        if (order->status() != OrderStatus::Preparing) {
            throw InvalidOrderException(
                "Order is not being prepared: " + orderStatusLabel(order->status()));
        }
        ticket.markReady();
        order->markReady();
        if (ticket.chef() != nullptr) {
            ticket.chef()->finishTicket();
        }
        return true;
    }
    return false;
}

bool KitchenService::serveNextReady() {
    return queue_.serveNextReady();
}

KitchenTicket* KitchenService::findForOrder(Entity::Id orderId) {
    for (auto& ticket : queue_.tickets()) {
        if (ticket.order()->id() == orderId) {
            return &ticket;
        }
    }
    return nullptr;
}

const KitchenTicket* KitchenService::findForOrder(Entity::Id orderId) const {
    for (const auto& ticket : queue_.tickets()) {
        if (ticket.order()->id() == orderId) {
            return &ticket;
        }
    }
    return nullptr;
}

std::vector<const KitchenTicket*> KitchenService::pendingTickets() const {
    std::vector<const KitchenTicket*> result;
    for (const auto& ticket : queue_.tickets()) {
        if (ticket.status() != KitchenTicket::Status::Served) {
            result.push_back(&ticket);
        }
    }
    return result;
}

std::size_t KitchenService::queuedCount() const {
    return queue_.queuedCount();
}

std::size_t KitchenService::size() const {
    return queue_.size();
}

KitchenQueue& KitchenService::queue() {
    return queue_;
}

const KitchenQueue& KitchenService::queue() const {
    return queue_;
}
