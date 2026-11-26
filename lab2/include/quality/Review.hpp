#ifndef REVIEW_HPP
#define REVIEW_HPP

#include "core/Entity.hpp"

#include <string>

class Review : public Entity {
public:
    Review(std::string customerName, int rating, std::string comment = {}, Entity::Id orderId = 0);

    const std::string& customerName() const;
    int rating() const;
    const std::string& comment() const;
    Entity::Id orderId() const;

    void setComment(std::string comment);
    bool isPositive() const;
    bool isNegative() const;
    std::string stars() const;
    std::string describe() const;

private:
    std::string customerName_;
    int rating_;
    std::string comment_;
    Entity::Id orderId_;
};

#endif
