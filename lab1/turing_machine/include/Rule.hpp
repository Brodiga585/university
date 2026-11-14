#ifndef RULE_HPP
#define RULE_HPP

#include <string>

class Rule {
private:
    std::string current_state_;
    char read_symbol_;
    std::string next_state_;
    char write_symbol_;
    char direction_;

public:
    Rule(std::string curr_st, char read_s, std::string next_st, char write_s, char dir);

    std::string getCurrentState() const;
    char getReadSymbol() const;
    std::string getNextState() const;
    char getWriteSymbol() const;
    char getDirection() const;

    bool operator==(const Rule& other) const;
};

#endif