#ifndef HALL_HPP
#define HALL_HPP

#include "core/Entity.hpp"
#include "venue/Reservation.hpp"
#include "venue/Table.hpp"

#include <cstddef>
#include <ctime>
#include <memory>
#include <string>
#include <vector>

class Hall {
public:
    Hall(std::string name, int capacity);

    const std::string& name() const;
    int capacity() const;
    int totalSeats() const;
    std::size_t tableCount() const;
    bool empty() const;

    Table& addTable(int number, int tableCapacity, std::string location);
    bool removeTable(int number);
    Table* findTable(int number);
    const Table* findTable(int number) const;
    std::vector<const Table*> freeTables(int guests) const;

    Reservation& book(int tableNumber,
                      std::string customerName,
                      std::string phone,
                      int guests,
                      std::time_t startTime,
                      int durationMinutes);
    Reservation* findReservation(Entity::Id id);
    const Reservation* findReservation(Entity::Id id) const;
    bool cancelReservation(Entity::Id id);
    std::vector<const Reservation*> activeReservations() const;
    const std::vector<Reservation>& reservations() const;

    std::string describe() const;

private:
    std::string name_;
    int capacity_;
    std::vector<std::unique_ptr<Table>> tables_;
    std::vector<Reservation> reservations_;
};

#endif
