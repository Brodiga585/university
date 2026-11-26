#ifndef EMPLOYEE_HPP
#define EMPLOYEE_HPP

#include "core/Money.hpp"
#include "people/Person.hpp"

#include <ctime>
#include <string>

class Employee : public Person {
public:
    enum class Status {
        Active,
        OnLeave,
        Dismissed
    };

    Employee(std::string fullName,
             std::string phone,
             std::string email,
             const Money& monthlySalary,
             std::time_t hiredAt,
             Status status = Status::Active);
    ~Employee() override = default;

    const Money& monthlySalary() const;
    std::time_t hiredAt() const;
    Status status() const;
    bool isActive() const;

    void setMonthlySalary(const Money& salary);
    void setStatus(Status status);

    int yearsOfService(std::time_t now) const;
    std::string statusLabel() const;
    std::string describe() const override;

protected:
    void ensureActive() const;

private:
    Money monthlySalary_;
    std::time_t hiredAt_;
    Status status_;
};

#endif
