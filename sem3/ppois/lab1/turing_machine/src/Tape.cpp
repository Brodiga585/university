#include "Tape.hpp"
#include <sstream>

Tape::Tape(char blank) : blank_symbol_(blank) {}

char Tape::operator[](int index) const {
    auto it = cells_.find(index);
    if (it != cells_.end()) {
        return it->second;
    }
    return blank_symbol_;
}

void Tape::write(int index, char value) {
    if (value == blank_symbol_) {
        cells_.erase(index);
    } else {
        cells_[index] = value;
    }
}

void Tape::loadFromString(const std::string& str) {
    cells_.clear();
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] != blank_symbol_) {
            cells_[static_cast<int>(i)] = str[i];
        }
    }
}

std::string Tape::toString(int head_pos) const {
    if (cells_.empty()) {
        return "[_]";
    }

    int min_idx = cells_.begin()->first;
    int max_idx = cells_.rbegin()->first;

    if (head_pos < min_idx) min_idx = head_pos;
    if (head_pos > max_idx) max_idx = head_pos;

    std::ostringstream oss;
    oss << "| ";
    for (int i = min_idx; i <= max_idx; ++i) {
        if (i == head_pos) {
            oss << "[" << (*this)[i] << "]";
        } else {
            oss << " " << (*this)[i] << " ";
        }
    }
    oss << " |";
    return oss.str();
}

bool Tape::operator==(const Tape& other) const {
    return this->cells_ == other.cells_ && this->blank_symbol_ == other.blank_symbol_;
}

bool Tape::operator!=(const Tape& other) const {
    return !(*this == other);
}