#pragma once

#include <string>

namespace BigNumber {

class bigNumber {
    using lenType = std::string::size_type;
public:
    bigNumber(const long long &number);
    bigNumber(const std::string &numberStr);
    ~bigNumber();

    bigNumber operator+(const bigNumber &n) const;
    bigNumber operator-(const bigNumber &n) const;

    bool operator<(const bigNumber &n) const;

    friend bigNumber abs(const bigNumber &n);
    lenType operator[](const lenType index) const;

    void print() const;
private:
    //positive integer add
    bigNumber positiveIntegerAdd(const bigNumber &n0, const bigNumber &n1) const;
    bigNumber positiveIntegerSubtract(const bigNumber &n0, const bigNumber &n1) const;

    bool isNegative() const;
    void initDecimalLen();

    lenType getIntLen() const;

    lenType c2Num(char ch) const {
        return static_cast<lenType>(ch - '0');
    }

    char n2Char(lenType i) const {
        return static_cast<char>(i + '0');
    }

private:
    std::string m_number;
    lenType m_decimalLen;
};

bigNumber abs(const bigNumber &n);

} /* namespace BigNumber */
