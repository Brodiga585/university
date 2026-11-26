#include "venue/Hall.hpp"

#include "exceptions/ReservationConflictException.hpp"
#include "exceptions/TableNotAvailableException.hpp"

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <utility>

Hall::Hall(std::string name, int capacity) : name_(std::move(name)), capacity_(capacity) {
    if (name_.empty()) {
        throw std::invalid_argument("Hall name must not be empty");
    }
    if (capacity_ <= 0) {
        throw std::invalid_argument("Hall capacity must be positive");
    }
}

const std::string& Hall::name() const {
    return name_;
}

int Hall::capacity() const {
    return capacity_;
}

int Hall::totalSeats() const {
    int total = 0;
    for (const auto& table : tables_) {
        total += table->capacity();
    }
    return total;
}

std::size_t Hall::tableCount() const {
    return tables_.size();
}

bool Hall::empty() const {
    return tables_.empty();
}

Table& Hall::addTable(int number, int tableCapacity, std::string location) {
    if (findTable(number) != nullptr) {
        throw std::invalid_argument("Duplicate table number: " + std::to_string(number));
    }
    if (totalSeats() + tableCapacity > capacity_) {
        throw std::invalid_argument("Hall capacity would be exceeded");
    }
    if (location.empty()) {
        location = name_;
    }
    tables_.push_back(std::make_unique<Table>(number, tableCapacity, std::move(location)));
    return *tables_.back();
}

bool Hall::removeTable(int number) {
    const auto position = std::find_if(
        tables_.begin(), tables_.end(),
        [number](const std::unique_ptr<Table>& table) {
            return table->number() == number;
        });
    if (position == tables_.end()) {
        return false;
    }
    const Entity::Id tableId = (*position)->id();
    const bool hasActiveReservation = std::any_of(
        reservations_.begin(), reservations_.end(),
        [tableId](const Reservation& reservation) {
            return reservation.table()->id() == tableId && reservation.isActive();
        });
    if (hasActiveReservation) {
        throw std::logic_error("Cannot remove a table with active reservations");
    }
    reservations_.erase(
        std::remove_if(
            reservations_.begin(), reservations_.end(),
            [tableId](const Reservation& reservation) {
                return reservation.table()->id() == tableId;
            }),
        reservations_.end());
    tables_.erase(position);
    return true;
}

Table* Hall::findTable(int number) {
    for (auto& table : tables_) {
        if (table->number() == number) {
            return table.get();
        }
    }
    return nullptr;
}

const Table* Hall::findTable(int number) const {
    for (const auto& table : tables_) {
        if (table->number() == number) {
            return table.get();
        }
    }
    return nullptr;
}

std::vector<const Table*> Hall::freeTables(int guests) const {
    if (guests <= 0) {
        throw std::invalid_argument("Guest count must be positive");
    }
    std::vector<const Table*> result;
    for (const auto& table : tables_) {
        if (table->isFree() && table->capacity() >= guests) {
            result.push_back(table.get());
        }
    }
    return result;
}

Reservation& Hall::book(int tableNumber,
                        std::string customerName,
                        std::string phone,
                        int guests,
                        std::time_t startTime,
                        int durationMinutes) {
    Table* table = findTable(tableNumber);
    if (table == nullptr) {
        throw std::invalid_argument("Unknown table number: " + std::to_string(tableNumber));
    }
    if (table->status() == Table::Status::Occupied) {
        throw TableNotAvailableException("Table " + std::to_string(tableNumber) +
                                         " is currently occupied");
    }
    if (guests <= 0 || table->capacity() < guests) {
        throw TableNotAvailableException(
            "Table " + std::to_string(tableNumber) + " cannot seat " +
            std::to_string(guests) + " guests");
    }
    for (const auto& reservation : reservations_) {
        if (reservation.isActive() && reservation.table() == table &&
            reservation.overlaps(startTime, durationMinutes)) {
            throw ReservationConflictException(
                "Table " + std::to_string(tableNumber) + " is already reserved for this time");
        }
    }
    reservations_.emplace_back(std::move(customerName), std::move(phone), table, guests,
                               startTime, durationMinutes);
    return reservations_.back();
}

Reservation* Hall::findReservation(Entity::Id id) {
    for (auto& reservation : reservations_) {
        if (reservation.id() == id) {
            return &reservation;
        }
    }
    return nullptr;
}

const Reservation* Hall::findReservation(Entity::Id id) const {
    for (const auto& reservation : reservations_) {
        if (reservation.id() == id) {
            return &reservation;
        }
    }
    return nullptr;
}

bool Hall::cancelReservation(Entity::Id id) {
    Reservation* reservation = findReservation(id);
    if (reservation == nullptr) {
        return false;
    }
    reservation->cancel();
    return true;
}

std::vector<const Reservation*> Hall::activeReservations() const {
    std::vector<const Reservation*> result;
    for (const auto& reservation : reservations_) {
        if (reservation.isActive()) {
            result.push_back(&reservation);
        }
    }
    return result;
}

const std::vector<Reservation>& Hall::reservations() const {
    return reservations_;
}

std::string Hall::describe() const {
    std::ostringstream output;
    output << "Hall '" << name_ << "': " << tables_.size() << " tables, " << totalSeats()
           << '/' << capacity_ << " seats, " << activeReservations().size()
           << " active reservations";
    return output.str();
}
