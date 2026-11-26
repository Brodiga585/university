#include "quality/Complaint.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

Complaint::Complaint(std::string customerName,
                     Category category,
                     std::string description,
                     Entity::Id orderId)
    : customerName_(std::move(customerName)),
      category_(category),
      description_(std::move(description)),
      orderId_(orderId),
      status_(Status::Open),
      resolvedAt_(0) {
    if (customerName_.empty()) {
        throw std::invalid_argument("Complaint customer name must not be empty");
    }
    if (description_.empty()) {
        throw std::invalid_argument("Complaint description must not be empty");
    }
}

const std::string& Complaint::customerName() const {
    return customerName_;
}

Complaint::Category Complaint::category() const {
    return category_;
}

const std::string& Complaint::description() const {
    return description_;
}

Entity::Id Complaint::orderId() const {
    return orderId_;
}

Complaint::Status Complaint::status() const {
    return status_;
}

const std::string& Complaint::resolution() const {
    return resolution_;
}

std::time_t Complaint::resolvedAt() const {
    return resolvedAt_;
}

bool Complaint::isOpen() const {
    return status_ == Status::Open || status_ == Status::InReview;
}

void Complaint::markInReview() {
    if (status_ != Status::Open) {
        throw std::logic_error("Only an open complaint can be taken into review");
    }
    status_ = Status::InReview;
}

void Complaint::resolve(std::string resolution) {
    if (!isOpen()) {
        throw std::logic_error("Complaint is already closed");
    }
    if (resolution.empty()) {
        throw std::invalid_argument("Complaint resolution must not be empty");
    }
    resolution_ = std::move(resolution);
    status_ = Status::Resolved;
    resolvedAt_ = std::time(nullptr);
}

void Complaint::reject(std::string reason) {
    if (!isOpen()) {
        throw std::logic_error("Complaint is already closed");
    }
    if (reason.empty()) {
        throw std::invalid_argument("Complaint rejection reason must not be empty");
    }
    resolution_ = std::move(reason);
    status_ = Status::Rejected;
    resolvedAt_ = std::time(nullptr);
}

std::string Complaint::categoryLabel() const {
    switch (category_) {
        case Category::Food:
            return "Food";
        case Category::Delivery:
            return "Delivery";
        case Category::Service:
            return "Service";
        case Category::Payment:
            return "Payment";
        case Category::Other:
            return "Other";
    }
    return "Unknown";
}

std::string Complaint::statusLabel() const {
    switch (status_) {
        case Status::Open:
            return "Open";
        case Status::InReview:
            return "In review";
        case Status::Resolved:
            return "Resolved";
        case Status::Rejected:
            return "Rejected";
    }
    return "Unknown";
}

std::string Complaint::describe() const {
    std::ostringstream output;
    output << "Complaint #" << id() << " by " << customerName_ << " [" << categoryLabel()
           << ", " << statusLabel() << "] " << description_;
    if (!resolution_.empty()) {
        output << " -> " << resolution_;
    }
    return output.str();
}
