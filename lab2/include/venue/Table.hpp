#ifndef TABLE_HPP
#define TABLE_HPP

#include "core/Entity.hpp"

#include <string>

class Table : public Entity {
public:
    enum class Status {
        Free,
        Reserved,
        Occupied
    };

    Table(int number, int capacity, std::string location);

    int number() const;
    int capacity() const;
    const std::string& location() const;
    Status status() const;
    int seatedGuests() const;

    bool isFree() const;
    bool canSeat(int guests) const;
    void reserve();
    void seat(int guests);
    void release();

    std::string statusLabel() const;
    std::string describe() const;

private:
    int number_;
    int capacity_;
    std::string location_;
    Status status_;
    int seatedGuests_;
};

#endif
