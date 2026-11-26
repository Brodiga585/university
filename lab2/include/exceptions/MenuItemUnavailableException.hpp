#ifndef MENU_ITEM_UNAVAILABLE_EXCEPTION_HPP
#define MENU_ITEM_UNAVAILABLE_EXCEPTION_HPP

#include "exceptions/PizzeriaException.hpp"

class MenuItemUnavailableException : public PizzeriaException {
public:
    explicit MenuItemUnavailableException(const std::string& message);
};

#endif
