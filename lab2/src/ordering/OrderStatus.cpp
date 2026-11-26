#include "ordering/OrderStatus.hpp"

std::string orderStatusLabel(OrderStatus status) {
    switch (status) {
        case OrderStatus::Draft:
            return "Draft";
        case OrderStatus::Placed:
            return "Placed";
        case OrderStatus::Confirmed:
            return "Confirmed";
        case OrderStatus::Preparing:
            return "Preparing";
        case OrderStatus::Ready:
            return "Ready";
        case OrderStatus::OutForDelivery:
            return "Out for delivery";
        case OrderStatus::Completed:
            return "Completed";
        case OrderStatus::Cancelled:
            return "Cancelled";
    }
    return "Unknown";
}

bool isFinalStatus(OrderStatus status) {
    return status == OrderStatus::Completed || status == OrderStatus::Cancelled;
}

bool isActiveStatus(OrderStatus status) {
    return !isFinalStatus(status) && status != OrderStatus::Draft;
}
