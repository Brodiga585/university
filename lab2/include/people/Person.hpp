#ifndef PERSON_HPP
#define PERSON_HPP

#include "core/Entity.hpp"

#include <string>

class Person : public Entity {
public:
    Person(std::string fullName, std::string phone, std::string email);
    ~Person() override = default;

    const std::string& fullName() const;
    const std::string& phone() const;
    const std::string& email() const;

    void setPhone(std::string phone);
    void setEmail(std::string email);

    virtual std::string role() const = 0;
    virtual std::string describe() const;

private:
    std::string fullName_;
    std::string phone_;
    std::string email_;
};

#endif
