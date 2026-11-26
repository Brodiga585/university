#ifndef CONSOLE_UI_HPP
#define CONSOLE_UI_HPP

#include <iostream>
#include <string>

class ConsoleUI {
public:
    ConsoleUI();
    ConsoleUI(std::istream& input, std::ostream& output);

    void printLine(const std::string& text) const;
    void printBlankLine() const;
    void printHeader(const std::string& title) const;
    void printSeparator() const;

    std::string readLine(const std::string& prompt) const;
    int readInt(const std::string& prompt, int minValue, int maxValue) const;
    double readDouble(const std::string& prompt, double minValue, double maxValue) const;
    bool readYesNo(const std::string& prompt) const;
    void waitForEnter() const;

private:
    std::istream& input_;
    std::ostream& output_;
    static constexpr int kSeparatorWidth = 60;
};

#endif
