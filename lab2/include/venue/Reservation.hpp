#ifndef RESERVATION_HPP
#define RESERVATION_HPP

#include "core/Entity.hpp"

#include <ctime>
#include <string>

class Table;

class Reservation : public Entity {
public:
    enum class Status {
        Booked,
        Seated,
        Completed,
        Cancelled,
        NoShow
    };

    Reservation(std::string customerName,
                std::string phone,
                Table* table,
                int guests,
                std::time_t startTime,
                int durationMinutes);

    const std::string& customerName() const;
    const std::string& phone() const;
    Table* table() const;
    int guests() const;
    std::time_t startTime() const;
    int durationMinutes() const;
    std::time_t endTime() const;
    Status status() const;
    bool isActive() const;

    void seat();
    void complete();
    void cancel();
    void markNoShow();

    bool overlaps(std::time_t start, int durationMinutes) const;
    std::string statusLabel() const;
    std::string describe() const;

private:
    std::string customerName_;
    std::string phone_;
    Table* table_;
    int guests_;
    std::time_t startTime_;
    int durationMinutes_;
    Status status_;
};

#endif
