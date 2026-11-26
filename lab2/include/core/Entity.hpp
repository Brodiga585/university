#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cstdint>
#include <ctime>

class Entity {
public:
    using Id = std::uint64_t;

    Entity();
    explicit Entity(Id id);
    virtual ~Entity() = default;

    Id id() const;
    std::time_t createdAt() const;
    bool sameIdentity(const Entity& other) const;

private:
    static Id nextId();

    Id id_;
    std::time_t createdAt_;
};

#endif
