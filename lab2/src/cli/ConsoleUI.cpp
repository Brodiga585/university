#include "cli/ConsoleUI.hpp"

#include <cctype>
#include <cmath>
#include <sstream>
#include <stdexcept>

ConsoleUI::ConsoleUI() : ConsoleUI(std::cin, std::cout) {}

ConsoleUI::ConsoleUI(std::istream& input, std::ostream& output)
    : input_(input), output_(output) {}

void ConsoleUI::printLine(const std::string& text) const {
    output_ << text << '\n';
}

void ConsoleUI::printBlankLine() const {
    output_ << '\n';
}

void ConsoleUI::printSeparator() const {
    output_ << std::string(kSeparatorWidth, '=') << '\n';
}

void ConsoleUI::printHeader(const std::string& title) const {
    printSeparator();
    output_ << title << '\n';
    printSeparator();
}

std::string ConsoleUI::readLine(const std::string& prompt) const {
    output_ << prompt;
    std::string line;
    if (!std::getline(input_, line)) {
        throw std::runtime_error("Input stream closed");
    }
    return line;
}

int ConsoleUI::readInt(const std::string& prompt, int minValue, int maxValue) const {
    while (true) {
        const std::string line = readLine(prompt);
        std::istringstream parser(line);
        int value = 0;
        char extra = '\0';
        const bool parsed = static_cast<bool>(parser >> value);
        const bool hasExtra = static_cast<bool>(parser >> extra);
        if (parsed && !hasExtra && value >= minValue && value <= maxValue) {
            return value;
        }
        output_ << "Invalid input. Enter a number from " << minValue
                << " to " << maxValue << ".\n";
    }
}

double ConsoleUI::readDouble(const std::string& prompt, double minValue, double maxValue) const {
    while (true) {
        const std::string line = readLine(prompt);
        std::istringstream parser(line);
        double value = 0.0;
        char extra = '\0';
        const bool parsed = static_cast<bool>(parser >> value);
        const bool hasExtra = static_cast<bool>(parser >> extra);
        if (parsed && !hasExtra && std::isfinite(value) &&
            value >= minValue && value <= maxValue) {
            return value;
        }
        output_ << "Invalid input. Enter a number from " << minValue
                << " to " << maxValue << ".\n";
    }
}

bool ConsoleUI::readYesNo(const std::string& prompt) const {
    while (true) {
        const std::string answer = readLine(prompt + " (y/n): ");
        if (!answer.empty()) {
            const char first = static_cast<char>(
                std::tolower(static_cast<unsigned char>(answer.front())));
            if (first == 'y') {
                return true;
            }
            if (first == 'n') {
                return false;
            }
        }
        output_ << "Please answer y or n.\n";
    }
}

void ConsoleUI::waitForEnter() const {
    output_ << "Press Enter to continue...";
    std::string ignored;
    std::getline(input_, ignored);
}
