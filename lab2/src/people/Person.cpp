#include "people/Person.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

Person::Person(std::string fullName, std::string phone, std::string email)
    : fullName_(std::move(fullName)), phone_(std::move(phone)), email_(std::move(email)) {
    if (fullName_.empty()) {
        throw std::invalid_argument("Person name must not be empty");
    }
    if (phone_.empty()) {
        throw std::invalid_argument("Person phone must not be empty");
    }
    setEmail(email_);
}

const std::string& Person::fullName() const {
    return fullName_;
}

const std::string& Person::phone() const {
    return phone_;
}

const std::string& Person::email() const {
    return email_;
}

void Person::setPhone(std::string phone) {
    if (phone.empty()) {
        throw std::invalid_argument("Person phone must not be empty");
    }
    phone_ = std::move(phone);
}

void Person::setEmail(std::string email) {
    if (!email.empty() && email.find('@') == std::string::npos) {
        throw std::invalid_argument("Person email must contain '@'");
    }
    email_ = std::move(email);
}

std::string Person::describe() const {
    std::ostringstream output;
    output << fullName_ << " [" << role() << "] " << phone_;
    if (!email_.empty()) {
        output << ", " << email_;
    }
    return output.str();
}
