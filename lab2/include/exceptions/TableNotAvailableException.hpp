#ifndef TABLE_NOT_AVAILABLE_EXCEPTION_HPP
#define TABLE_NOT_AVAILABLE_EXCEPTION_HPP

#include "exceptions/PizzeriaException.hpp"

class TableNotAvailableException : public PizzeriaException {
public:
    explicit TableNotAvailableException(const std::string& message);
};

#endif
