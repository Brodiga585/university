#include "ordering/TakeawayOrder.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

TakeawayOrder::TakeawayOrder(std::string customerName,
                             std::string pickupCode,
                             std::time_t readyAt,
                             Money packagingFee)
    : Order(std::move(customerName)),
      pickupCode_(std::move(pickupCode)),
      readyAt_(readyAt),
      pickedUpAt_(0),
      packagingFee_(std::move(packagingFee)) {
    if (pickupCode_.empty()) {
        pickupCode_ = "T-" + std::to_string(id());
    }
    if (readyAt_ < 0) {
        throw std::invalid_argument("Ready time must not be negative");
    }
    if (packagingFee_.minorUnits() < 0) {
        throw std::invalid_argument("Packaging fee must not be negative");
    }
}

const std::string& TakeawayOrder::pickupCode() const {
    return pickupCode_;
}

std::time_t TakeawayOrder::readyAt() const {
    return readyAt_;
}

std::time_t TakeawayOrder::pickedUpAt() const {
    return pickedUpAt_;
}

const Money& TakeawayOrder::packagingFee() const {
    return packagingFee_;
}

void TakeawayOrder::setReadyAt(std::time_t readyAt) {
    if (readyAt < 0) {
        throw std::invalid_argument("Ready time must not be negative");
    }
    readyAt_ = readyAt;
}

void TakeawayOrder::setPackagingFee(const Money& packagingFee) {
    if (packagingFee.minorUnits() < 0) {
        throw std::invalid_argument("Packaging fee must not be negative");
    }
    packagingFee_ = packagingFee;
}

Money TakeawayOrder::extraFee() const {
    return packagingFee_;
}

void TakeawayOrder::complete() {
    Order::complete();
    pickedUpAt_ = std::time(nullptr);
}

std::string TakeawayOrder::orderType() const {
    return "Takeaway";
}

std::string TakeawayOrder::describe() const {
    std::ostringstream output;
    output << Order::describe() << " | pickup code " << pickupCode_
           << ", packaging " << packagingFee_.toString();
    return output.str();
}
