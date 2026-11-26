#ifndef ORDER_STATUS_HPP
#define ORDER_STATUS_HPP

#include <string>

enum class OrderStatus {
    Draft,
    Placed,
    Confirmed,
    Preparing,
    Ready,
    OutForDelivery,
    Completed,
    Cancelled
};

std::string orderStatusLabel(OrderStatus status);
bool isFinalStatus(OrderStatus status);
bool isActiveStatus(OrderStatus status);

#endif
