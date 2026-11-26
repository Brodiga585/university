#include "venue/Table.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

Table::Table(int number, int capacity, std::string location)
    : number_(number),
      capacity_(capacity),
      location_(std::move(location)),
      status_(Status::Free),
      seatedGuests_(0) {
    if (number_ <= 0) {
        throw std::invalid_argument("Table number must be positive");
    }
    if (capacity_ <= 0) {
        throw std::invalid_argument("Table capacity must be positive");
    }
    if (location_.empty()) {
        throw std::invalid_argument("Table location must not be empty");
    }
}

int Table::number() const {
    return number_;
}

int Table::capacity() const {
    return capacity_;
}

const std::string& Table::location() const {
    return location_;
}

Table::Status Table::status() const {
    return status_;
}

int Table::seatedGuests() const {
    return seatedGuests_;
}

bool Table::isFree() const {
    return status_ == Status::Free;
}

bool Table::canSeat(int guests) const {
    return guests > 0 && guests <= capacity_ && status_ != Status::Occupied;
}

void Table::reserve() {
    if (status_ != Status::Free) {
        throw std::logic_error("Table " + std::to_string(number_) + " is not free");
    }
    status_ = Status::Reserved;
}

void Table::seat(int guests) {
    if (!canSeat(guests)) {
        throw std::logic_error("Table " + std::to_string(number_) +
                               " cannot seat the requested number of guests");
    }
    status_ = Status::Occupied;
    seatedGuests_ = guests;
}

void Table::release() {
    if (status_ == Status::Free) {
        throw std::logic_error("Table " + std::to_string(number_) + " is already free");
    }
    status_ = Status::Free;
    seatedGuests_ = 0;
}

std::string Table::statusLabel() const {
    switch (status_) {
        case Status::Free:
            return "Free";
        case Status::Reserved:
            return "Reserved";
        case Status::Occupied:
            return "Occupied";
    }
    return "Unknown";
}

std::string Table::describe() const {
    std::ostringstream output;
    output << "Table " << number_ << " (" << capacity_ << " seats, " << location_
           << ") [" << statusLabel() << ']';
    if (status_ == Status::Occupied) {
        output << ", " << seatedGuests_ << " guests";
    }
    return output.str();
}
