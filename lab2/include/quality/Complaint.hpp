#ifndef COMPLAINT_HPP
#define COMPLAINT_HPP

#include "core/Entity.hpp"

#include <ctime>
#include <string>

class Complaint : public Entity {
public:
    enum class Category {
        Food,
        Delivery,
        Service,
        Payment,
        Other
    };

    enum class Status {
        Open,
        InReview,
        Resolved,
        Rejected
    };

    Complaint(std::string customerName,
              Category category,
              std::string description,
              Entity::Id orderId = 0);

    const std::string& customerName() const;
    Category category() const;
    const std::string& description() const;
    Entity::Id orderId() const;
    Status status() const;
    const std::string& resolution() const;
    std::time_t resolvedAt() const;

    bool isOpen() const;
    void markInReview();
    void resolve(std::string resolution);
    void reject(std::string reason);

    std::string categoryLabel() const;
    std::string statusLabel() const;
    std::string describe() const;

private:
    std::string customerName_;
    Category category_;
    std::string description_;
    Entity::Id orderId_;
    Status status_;
    std::string resolution_;
    std::time_t resolvedAt_;
};

#endif
