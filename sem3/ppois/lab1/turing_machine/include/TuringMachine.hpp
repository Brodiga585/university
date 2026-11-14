#ifndef TURING_MACHINE_HPP
#define TURING_MACHINE_HPP

#include "Tape.hpp"
#include "Head.hpp"
#include "Rule.hpp"
#include <vector>
#include <string>

class TuringMachine {
private:
    Tape tape_;
    Head head_;
    std::vector<Rule> rules_;
    std::string current_state_;
    std::string halt_state_;

public:
    TuringMachine();

    void setTape(const Tape& tape);
    void setHead(const Head& head);
    void addRule(const Rule& rule);
    void setCurrentState(const std::string& state);
    void setHaltState(const std::string& state);

    const Tape& getTape() const;
    const Head& getHead() const;
    std::string getCurrentState() const;

    void loadFromFile(const std::string& filename);
    bool step();
    int run(bool log_enabled = false);
};

#endif