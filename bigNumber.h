#pragma once

#include <string>

namespace BigNumber {

class bigNumber;

extern const bigNumber ZERO;
extern const bigNumber ONE;
extern const bigNumber EMT;

class bigNumber {
    using lenType = std::string::size_type;
public:
    bigNumber();
    bigNumber(const std::string &numberStr);
    ~bigNumber();

    bigNumber operator+(const bigNumber &n) const;
    bigNumber operator-(const bigNumber &n) const;
    bigNumber operator*(const bigNumber &n) const;
    bigNumber operator/(const bigNumber &n) const;
    bigNumber operator%(const bigNumber &n) const;

    bigNumber pow(int n) const;

    bool operator<(const bigNumber &n) const;
    bool operator>(const bigNumber &n) const;
    bool operator==(const bigNumber &n) const;
    bool operator!=(const bigNumber &n) const;

    bool operator<=(const bigNumber &n) const;
    bool operator>=(const bigNumber &n) const;

    friend bigNumber abs(const bigNumber &n);
    friend bigNumber negation(const bigNumber &n);
    friend void alignment(bigNumber &n0, bigNumber &n1);
    int operator[](const lenType index) const;

    void print() const;
    lenType getd() const {
        return m_decimalLen;
    }
private:
    //positive integer add
    bigNumber positiveIntegerAdd(const bigNumber &n0, const bigNumber &n1) const;
    bigNumber positiveIntegerSubtract(const bigNumber &n0, const bigNumber &n1) const;
    bigNumber positiveIntegerMultiply(bigNumber num, int n) const;
    bigNumber karatsuba(const bigNumber &n0, const bigNumber &n1) const;
    bigNumber modulo(const bigNumber &dividend, const bigNumber &divisor, bigNumber &remainder) const;
    bigNumber div(const bigNumber &dividend, const bigNumber &divisor, lenType precision) const;

    bigNumber toPositiveInteger() const;
    bool isNegative() const;
    void fixNumber();

    lenType getIntLen() const;

    int c2Num(char ch) const {
        return static_cast<int>(ch - '0');
    }

    char n2Char(int i) const {
        return static_cast<char>(i + '0');
    }

private:
    std::string m_number;
    lenType m_decimalLen;
};

bigNumber abs(const bigNumber &n);
bigNumber negation(const bigNumber &n);
void alignment(bigNumber &n0, bigNumber &n1);

} /* namespace BigNumber */
