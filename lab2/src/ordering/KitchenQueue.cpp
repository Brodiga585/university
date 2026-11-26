#include "ordering/KitchenQueue.hpp"

#include <cstddef>

void KitchenQueue::enqueue(const KitchenTicket& ticket) {
    tickets_.push_back(ticket);
}

KitchenTicket* KitchenQueue::next() {
    KitchenTicket* best = nullptr;
    for (auto& ticket : tickets_) {
        if (ticket.status() != KitchenTicket::Status::Queued) {
            continue;
        }
        if (best == nullptr || ticket.priority() > best->priority() ||
            (ticket.priority() == best->priority() &&
             ticket.createdAt() < best->createdAt())) {
            best = &ticket;
        }
    }
    return best;
}

const KitchenTicket* KitchenQueue::next() const {
    const KitchenTicket* best = nullptr;
    for (const auto& ticket : tickets_) {
        if (ticket.status() != KitchenTicket::Status::Queued) {
            continue;
        }
        if (best == nullptr || ticket.priority() > best->priority() ||
            (ticket.priority() == best->priority() &&
             ticket.createdAt() < best->createdAt())) {
            best = &ticket;
        }
    }
    return best;
}

bool KitchenQueue::startNext() {
    KitchenTicket* ticket = next();
    if (ticket == nullptr) {
        return false;
    }
    ticket->start();
    return true;
}

bool KitchenQueue::serveNextReady() {
    for (auto& ticket : tickets_) {
        if (ticket.status() == KitchenTicket::Status::Ready) {
            ticket.serve();
            return true;
        }
    }
    return false;
}

std::size_t KitchenQueue::size() const {
    return tickets_.size();
}

std::size_t KitchenQueue::queuedCount() const {
    std::size_t count = 0;
    for (const auto& ticket : tickets_) {
        if (ticket.status() == KitchenTicket::Status::Queued) {
            ++count;
        }
    }
    return count;
}

bool KitchenQueue::empty() const {
    return tickets_.empty();
}

std::vector<KitchenTicket>& KitchenQueue::tickets() {
    return tickets_;
}

const std::vector<KitchenTicket>& KitchenQueue::tickets() const {
    return tickets_;
}
