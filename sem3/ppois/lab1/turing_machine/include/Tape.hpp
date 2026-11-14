#ifndef TAPE_HPP
#define TAPE_HPP

#include <map>
#include <iostream>
#include <string>

class Tape {
private:
    std::map<int, char> cells_;
    char blank_symbol_;

public:
    explicit Tape(char blank = '_');

    char operator[](int index) const;
    void write(int index, char value);
    void loadFromString(const std::string& str);
    std::string toString(int head_pos) const;

    bool operator==(const Tape& other) const;
    bool operator!=(const Tape& other) const;
};

#endif