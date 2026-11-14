#include "Head.hpp"

Head::Head(int start_pos) : position_(start_pos) {}

int Head::getPosition() const {
    return position_;
}

void Head::setPosition(int pos) {
    position_ = pos;
}

Head& Head::operator++() {
    ++position_;
    return *this;
}

Head Head::operator++(int) {
    Head temp = *this;
    ++position_;
    return temp;
}

Head& Head::operator--() {
    --position_;
    return *this;
}

Head Head::operator--(int) {
    Head temp = *this;
    --position_;
    return temp;
}

bool Head::operator==(const Head& other) const {
    return position_ == other.position_;
}

bool Head::operator!=(const Head& other) const {
    return !(*this == other);
}