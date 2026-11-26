#ifndef MONEY_HPP
#define MONEY_HPP

#include <iosfwd>
#include <string>

class Money {
public:
    Money();
    explicit Money(long long minorUnits, std::string currency = "RUB");

    static Money fromRubles(double rubles, const std::string& currency = "RUB");

    long long minorUnits() const;
    double toRubles() const;
    const std::string& currency() const;

    Money operator+(const Money& other) const;
    Money operator-(const Money& other) const;
    Money operator*(double factor) const;
    Money& operator+=(const Money& other);

    bool operator==(const Money& other) const;
    bool operator!=(const Money& other) const;
    bool operator<(const Money& other) const;
    bool operator>(const Money& other) const;

    std::string toString() const;

private:
    void ensureSameCurrency(const Money& other) const;

    long long minorUnits_;
    std::string currency_;
};

std::ostream& operator<<(std::ostream& output, const Money& money);

#endif
