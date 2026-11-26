#include "venue/Reservation.hpp"

#include "venue/Table.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

namespace {
constexpr double kSecondsPerMinute = 60.0;
}

Reservation::Reservation(std::string customerName,
                         std::string phone,
                         Table* table,
                         int guests,
                         std::time_t startTime,
                         int durationMinutes)
    : customerName_(std::move(customerName)),
      phone_(std::move(phone)),
      table_(table),
      guests_(guests),
      startTime_(startTime),
      durationMinutes_(durationMinutes),
      status_(Status::Booked) {
    if (customerName_.empty()) {
        throw std::invalid_argument("Reservation customer name must not be empty");
    }
    if (phone_.empty()) {
        throw std::invalid_argument("Reservation phone must not be empty");
    }
    if (table_ == nullptr) {
        throw std::invalid_argument("Reservation table must not be null");
    }
    if (guests_ <= 0 || guests_ > table_->capacity()) {
        throw std::invalid_argument("Reservation guest count exceeds table capacity");
    }
    if (startTime_ <= 0) {
        throw std::invalid_argument("Reservation start time must be positive");
    }
    if (durationMinutes_ <= 0) {
        throw std::invalid_argument("Reservation duration must be positive");
    }
}

const std::string& Reservation::customerName() const {
    return customerName_;
}

const std::string& Reservation::phone() const {
    return phone_;
}

Table* Reservation::table() const {
    return table_;
}

int Reservation::guests() const {
    return guests_;
}

std::time_t Reservation::startTime() const {
    return startTime_;
}

int Reservation::durationMinutes() const {
    return durationMinutes_;
}

std::time_t Reservation::endTime() const {
    return startTime_ +
           static_cast<std::time_t>(durationMinutes_ * kSecondsPerMinute);
}

Reservation::Status Reservation::status() const {
    return status_;
}

bool Reservation::isActive() const {
    return status_ == Status::Booked || status_ == Status::Seated;
}

void Reservation::seat() {
    if (status_ != Status::Booked) {
        throw std::logic_error("Only a booked reservation can be seated");
    }
    status_ = Status::Seated;
}

void Reservation::complete() {
    if (status_ != Status::Seated) {
        throw std::logic_error("Only a seated reservation can be completed");
    }
    status_ = Status::Completed;
}

void Reservation::cancel() {
    if (status_ != Status::Booked) {
        throw std::logic_error("Only a booked reservation can be cancelled");
    }
    status_ = Status::Cancelled;
}

void Reservation::markNoShow() {
    if (status_ != Status::Booked) {
        throw std::logic_error("Only a booked reservation can be marked as no-show");
    }
    status_ = Status::NoShow;
}

bool Reservation::overlaps(std::time_t start, int durationMinutes) const {
    if (durationMinutes <= 0) {
        return false;
    }
    const std::time_t otherEnd =
        start + static_cast<std::time_t>(durationMinutes * kSecondsPerMinute);
    return start < endTime() && startTime_ < otherEnd;
}

std::string Reservation::statusLabel() const {
    switch (status_) {
        case Status::Booked:
            return "Booked";
        case Status::Seated:
            return "Seated";
        case Status::Completed:
            return "Completed";
        case Status::Cancelled:
            return "Cancelled";
        case Status::NoShow:
            return "No-show";
    }
    return "Unknown";
}

std::string Reservation::describe() const {
    std::ostringstream output;
    output << "Reservation #" << id() << " for " << customerName_ << " (" << phone_
           << "), " << guests_ << " guests, table " << table_->number()
           << " [" << statusLabel() << ']';
    return output.str();
}
