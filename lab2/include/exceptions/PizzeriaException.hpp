#ifndef PIZZERIA_EXCEPTION_HPP
#define PIZZERIA_EXCEPTION_HPP

#include <stdexcept>
#include <string>

class PizzeriaException : public std::runtime_error {
public:
    explicit PizzeriaException(const std::string& message, int code = 0);

    int code() const;

private:
    int code_;
};

#endif
