#include "Rule.hpp"

Rule::Rule(std::string curr_st, char read_s, std::string next_st, char write_s, char dir)
    : current_state_(curr_st), read_symbol_(read_s), next_state_(next_st), write_symbol_(write_s), direction_(dir) {}

std::string Rule::getCurrentState() const { return current_state_; }
char Rule::getReadSymbol() const { return read_symbol_; }
std::string Rule::getNextState() const { return next_state_; }
char Rule::getWriteSymbol() const { return write_symbol_; }
char Rule::getDirection() const { return direction_; }

bool Rule::operator==(const Rule& other) const {
    return current_state_ == other.current_state_ &&
           read_symbol_ == other.read_symbol_ &&
           next_state_ == other.next_state_ &&
           write_symbol_ == other.write_symbol_ &&
           direction_ == other.direction_;
}