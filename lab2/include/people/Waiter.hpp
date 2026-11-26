#ifndef WAITER_HPP
#define WAITER_HPP

#include "people/Employee.hpp"

class Waiter : public Employee {
public:
    Waiter(std::string fullName,
           std::string phone,
           std::string email,
           const Money& monthlySalary,
           std::time_t hiredAt,
           int maxTables = 4);

    int maxTables() const;
    int assignedTables() const;
    bool canTakeTable() const;
    const Money& tips() const;

    void assignTable();
    void releaseTable();
    void addTip(const Money& tip);

    std::string role() const override;
    std::string describe() const override;

private:
    int maxTables_;
    int assignedTables_;
    Money tips_;
};

#endif
