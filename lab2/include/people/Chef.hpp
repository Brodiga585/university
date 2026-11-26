#ifndef CHEF_HPP
#define CHEF_HPP

#include "people/Employee.hpp"

#include <string>

class Chef : public Employee {
public:
    enum class SkillLevel {
        Commis,
        ChefDePartie,
        SousChef,
        HeadChef
    };

    Chef(std::string fullName,
         std::string phone,
         std::string email,
         const Money& monthlySalary,
         std::time_t hiredAt,
         SkillLevel skillLevel,
         std::string specialty);

    SkillLevel skillLevel() const;
    const std::string& specialty() const;
    int activeTickets() const;
    int capacity() const;
    bool canTakeTicket() const;

    void beginTicket();
    void finishTicket();

    std::string role() const override;
    std::string describe() const override;

private:
    SkillLevel skillLevel_;
    std::string specialty_;
    int activeTickets_;
};

#endif
