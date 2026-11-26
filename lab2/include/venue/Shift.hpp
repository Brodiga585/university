#ifndef SHIFT_HPP
#define SHIFT_HPP

#include "core/Entity.hpp"
#include "core/Money.hpp"

#include <cstddef>
#include <ctime>
#include <string>
#include <vector>

class Employee;

class Shift : public Entity {
public:
    Shift(std::string title, std::time_t startsAt, int durationMinutes);

    const std::string& title() const;
    std::time_t startsAt() const;
    std::time_t endsAt() const;
    int durationMinutes() const;
    bool isOpen() const;

    void addEmployee(Employee* employee);
    bool removeEmployee(Employee* employee);
    const std::vector<Employee*>& employees() const;
    std::size_t employeeCount() const;

    bool covers(std::time_t moment) const;
    Money payrollCost() const;
    void close();
    std::string describe() const;

private:
    std::string title_;
    std::time_t startsAt_;
    int durationMinutes_;
    bool open_;
    std::vector<Employee*> employees_;
};

#endif
