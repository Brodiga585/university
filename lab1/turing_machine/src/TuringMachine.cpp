#include "TuringMachine.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

TuringMachine::TuringMachine() : current_state_("q0"), halt_state_("halt") {}

void TuringMachine::setTape(const Tape& tape) { tape_ = tape; }
void TuringMachine::setHead(const Head& head) { head_ = head; }
void TuringMachine::addRule(const Rule& rule) { rules_.push_back(rule); }
void TuringMachine::setCurrentState(const std::string& state) { current_state_ = state; }
void TuringMachine::setHaltState(const std::string& state) { halt_state_ = state; }

const Tape& TuringMachine::getTape() const { return tape_; }
const Head& TuringMachine::getHead() const { return head_; }
std::string TuringMachine::getCurrentState() const { return current_state_; }

void TuringMachine::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    rules_.clear();

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "state:") {
            iss >> current_state_;
        } else if (type == "tape:") {
            std::string tape_str;
            iss >> tape_str;
            tape_.loadFromString(tape_str);
        } else if (type == "head:") {
            int pos;
            iss >> pos;
            head_.setPosition(pos);
        } else {
            std::string curr_st, next_st;
            char read_s, write_s, dir;
            curr_st = type;
            if (iss >> read_s >> next_st >> write_s >> dir) {
                rules_.emplace_back(curr_st, read_s, next_st, write_s, dir);
            }
        }
    }
}

bool TuringMachine::step() {
    if (current_state_ == halt_state_) {
        return false;
    }

    char current_char = tape_[head_.getPosition()];
    const Rule* matched_rule = nullptr;

    for (const auto& rule : rules_) {
        if (rule.getCurrentState() == current_state_ && rule.getReadSymbol() == current_char) {
            matched_rule = &rule;
            break;
        }
    }

    if (!matched_rule) {
        return false;
    }

    tape_.write(head_.getPosition(), matched_rule->getWriteSymbol());
    current_state_ = matched_rule->getNextState();

    char dir = matched_rule->getDirection();
    if (dir == 'R' || dir == 'r') {
        ++head_;
    } else if (dir == 'L' || dir == 'l') {
        --head_;
    }

    return true;
}

int TuringMachine::run(bool log_enabled) {
    int steps = 0;
    if (log_enabled) {
        std::cout << "[Initial State] State: " << current_state_ << " | Tape: " << tape_.toString(head_.getPosition()) << "\n";
    }

    while (current_state_ != halt_state_ && step()) {
        steps++;
        if (log_enabled) {
            std::cout << "[Step " << steps << "] State: " << current_state_ << " | Tape: " << tape_.toString(head_.getPosition()) << "\n";
        }
    }

    if (log_enabled) {
        std::cout << "[Finished] Total steps: " << steps << "\n";
    }
    return steps;
}