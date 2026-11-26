#include "quality/Review.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
constexpr int kMinimumRating = 1;
constexpr int kMaximumRating = 5;
constexpr int kPositiveThreshold = 4;
constexpr int kNegativeThreshold = 2;
}

Review::Review(std::string customerName, int rating, std::string comment, Entity::Id orderId)
    : customerName_(std::move(customerName)),
      rating_(rating),
      comment_(std::move(comment)),
      orderId_(orderId) {
    if (customerName_.empty()) {
        throw std::invalid_argument("Review customer name must not be empty");
    }
    if (rating_ < kMinimumRating || rating_ > kMaximumRating) {
        throw std::invalid_argument("Review rating must be between 1 and 5");
    }
}

const std::string& Review::customerName() const {
    return customerName_;
}

int Review::rating() const {
    return rating_;
}

const std::string& Review::comment() const {
    return comment_;
}

Entity::Id Review::orderId() const {
    return orderId_;
}

void Review::setComment(std::string comment) {
    comment_ = std::move(comment);
}

bool Review::isPositive() const {
    return rating_ >= kPositiveThreshold;
}

bool Review::isNegative() const {
    return rating_ <= kNegativeThreshold;
}

std::string Review::stars() const {
    return std::string(static_cast<std::size_t>(rating_), '*') +
           std::string(static_cast<std::size_t>(kMaximumRating - rating_), '-');
}

std::string Review::describe() const {
    std::ostringstream output;
    output << "Review #" << id() << " by " << customerName_ << " [" << stars() << "] "
           << rating_ << "/5";
    if (orderId_ != 0) {
        output << " for order #" << orderId_;
    }
    if (!comment_.empty()) {
        output << ": " << comment_;
    }
    return output.str();
}
