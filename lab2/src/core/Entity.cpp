#include "core/Entity.hpp"

#include <atomic>
#include <stdexcept>

namespace {
std::atomic<Entity::Id> nextEntityId{1};
}

Entity::Entity() : id_(nextId()), createdAt_(std::time(nullptr)) {}

Entity::Entity(Id id) : id_(id), createdAt_(std::time(nullptr)) {
    if (id_ == 0) {
        throw std::invalid_argument("Entity id must be positive");
    }
}

Entity::Id Entity::id() const {
    return id_;
}

std::time_t Entity::createdAt() const {
    return createdAt_;
}

bool Entity::sameIdentity(const Entity& other) const {
    return id_ == other.id_;
}

Entity::Id Entity::nextId() {
    return nextEntityId.fetch_add(1, std::memory_order_relaxed);
}
