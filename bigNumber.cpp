#include <iostream>
#include <algorithm>

#include "bigNumber.h"

namespace BigNumber {

const bigNumber ZERO("0");
const bigNumber ONE("1");
const bigNumber EMT;

bigNumber::bigNumber() : m_number(""), m_decimalLen(0) {}

bigNumber::bigNumber(const std::string &numberStr) : m_number(numberStr) {
    fixNumber();
}

bigNumber::~bigNumber() {}

bigNumber bigNumber::operator+(const bigNumber &n) const  {
    if (ZERO == *this) return n;
    if (ZERO == n) return *this;

    const auto dLen0 = m_decimalLen;
    const auto dLen1 = n.m_decimalLen;
    bigNumber abs0 = abs(*this), abs1 = abs(n);
    alignment(abs0, abs1);
    const auto &numMax = abs0 > abs1 ? abs0 : abs1;
    const auto &numMin = abs0 < abs1 ? abs0 : abs1;

    bigNumber res;
    bool resultIsNegative = false;

    if (isNegative() == n.isNegative()) {
        resultIsNegative = isNegative();
        res = positiveIntegerAdd(numMax, numMin);
    } else {
        resultIsNegative = abs(*this) > abs(n) ? isNegative() : n.isNegative();
        res = positiveIntegerSubtract(numMax, numMin);
    }
    if (dLen0 != 0 || dLen1 != 0) {
        res.m_number.insert(res.m_number.size() - (dLen0 > dLen1 ? dLen0 : dLen1), 1, '.');
    }
    if (resultIsNegative) {
        res.m_number = "-" + res.m_number;
    }
    //整数减法也有可能产生前置无效0,故需要统一fixNumber
    res.fixNumber();
    return res;
}

bigNumber bigNumber::operator-(const bigNumber &n) const {
    if (ZERO == *this) return negation(n);
    if (ZERO == n) return *this;

    const auto dLen0 = m_decimalLen;
    const auto dLen1 = n.m_decimalLen;
    bigNumber abs0 = abs(*this), abs1 = abs(n);
    alignment(abs0, abs1);
    const auto &numMax = abs0 > abs1 ? abs0 : abs1;
    const auto &numMin = abs0 < abs1 ? abs0 : abs1;

    bigNumber res;
    bool resultIsNegative = false;

    if (isNegative() == n.isNegative()) {
        resultIsNegative = (isNegative() == (abs(*this) > abs(n)));
        res = positiveIntegerSubtract(numMax, numMin);
    } else {
        resultIsNegative = isNegative();
        res = positiveIntegerAdd(numMax, numMin);
    }
    if (dLen0 != 0 || dLen1 != 0) {
        res.m_number.insert(res.m_number.size() - (dLen0 > dLen1 ? dLen0 : dLen1), 1, '.');
    }
    if (resultIsNegative) {
        res.m_number = "-" + res.m_number;
    }
    //整数减法也有可能产生前置无效0,故需要统一fixNumber
    res.fixNumber();
    return res;
}

bigNumber bigNumber::operator*(const bigNumber &n) const {
    if (ZERO == *this || ZERO == n) return ZERO;
    bool resultIsNegative = isNegative() != n.isNegative();
    if (ONE == abs(*this)) return resultIsNegative  == n.isNegative() ? n : negation(n);
    if (ONE == abs(n)) return resultIsNegative == isNegative() ? *this : negation(*this);

    const auto dLen0 = m_decimalLen;
    const auto dLen1 = n.m_decimalLen;
    const auto abs0 = abs(*this);
    const auto abs1 = abs(n);

    bigNumber res;

    if (0 == dLen0 && 0 == dLen1) {
        res = karatsuba(abs0, abs1);
    } else {
        const auto integer0 = abs0.toPositiveInteger();
        const auto integer1 = abs1.toPositiveInteger();
        res = karatsuba(integer0, integer1);
        if (res.m_number.size() <= dLen0 + dLen1) {
            std::string fixStr = "0";
            for (lenType i = 0; i < dLen0 + dLen1 - res.m_number.size(); ++i) {
                fixStr.push_back('0');
            }
            res.m_number = fixStr + res.m_number;
        }
        res.m_number.insert(res.m_number.size() - (dLen0 + dLen1), 1, '.');
        res.fixNumber();
    }
    if (resultIsNegative && res.m_number != "0") res.m_number = "-" + res.m_number;
    return res;
}

bigNumber bigNumber::operator%(const bigNumber &n) const {
    if (m_decimalLen != 0 || n.m_decimalLen != 0) std::cerr << "% support integer only." << std::endl;
    if (ZERO == n) std::cerr << "Divisor can't be 0!" << std::endl;

    if (ZERO == *this || ONE == abs(n) || abs(*this) == abs(n)) return ZERO;

    const auto abs0 = abs(*this);
    const auto abs1 = abs(n);
    bigNumber remainder;
    modulo(abs0, abs1, remainder);
    if (isNegative()) remainder.m_number = "-" + remainder.m_number;
    return remainder;
}

bigNumber bigNumber::operator/(const bigNumber &n) const {
    if (ZERO == n) std::cerr << "Divisor can't be 0!" << std::endl;

    if (ZERO == *this) return ZERO;
    const bool resultIsNegative = isNegative() != n.isNegative();
    if (ONE == abs(n)) {
        bigNumber res = abs(*this);
        if (resultIsNegative) res.m_number = "-" + res.m_number;
        return res;
    }
    if (abs(*this) == abs(n)) return bigNumber(resultIsNegative ? "-1" : "1");

    //取模
    auto abs0 = abs(*this);
    auto abs1 = abs(n);
    if (0 == m_decimalLen && 0 == n.m_decimalLen) {
        bigNumber remainder;
        bigNumber res = modulo(abs0, abs1, remainder);
        if (resultIsNegative && res != ZERO) res.m_number = "-" + res.m_number;
        return res;
    }

    //除法
    const auto dLen0 = m_decimalLen;
    const auto dLen1 = n.m_decimalLen;
    alignment(abs0, abs1);
    lenType precision = 21;
    auto res = div(abs0, abs1, precision);
    res.m_number.insert(res.m_number.size() - precision, 1, '.');
    res.fixNumber();
    if (resultIsNegative) res.m_number = "-" + res.m_number;
    return res;
}

bigNumber bigNumber::pow(int n) const {
    if (n < 0) std::cerr << "pow function don't support negative integer." << std::endl;
    if (0 == n) return ONE;
    if (1 == n) return *this;

    bigNumber res;
    if (2 == n) {
        res = (*this) * (*this);
        return res;
    }

    res = pow(n / 2) * pow(n - n / 2);
    return res;
}

bool bigNumber::operator<(const bigNumber &n) const {
    if (!isNegative() && !n.isNegative()) {
        if (getIntLen() < n.getIntLen()) {
            return true;
        } else if (getIntLen() > n.getIntLen()) {
            return false;
        } else {
            const auto len0 = m_number.size();
            const auto len1 = n.m_number.size();
            const auto lenMax = len0 > len1 ? len0 : len1;
            const auto lenMin = len0 < len1 ? len0 : len1;
            bool isThisSmaller = false;
            for (lenType i = 0; i < lenMax; ++i) {
                if (getIntLen() == i) continue;
                if (i < lenMin) {
                    if (c2Num(m_number[i]) == c2Num(n.m_number[i])) continue;
                    isThisSmaller = c2Num(m_number[i]) < c2Num(n.m_number[i]);
                    break;
                } else {
                    isThisSmaller =  len0 < len1;
                    break;
                }
            }
            return isThisSmaller;
        }
    } else if (!isNegative() && n.isNegative()) {
        return false;
    } else if (isNegative() && !n.isNegative()) {
        return true;
    } else {
        return abs(*this) > abs(n);
    }
}

bool bigNumber::operator>(const bigNumber &n) const {
    if (!isNegative() && !n.isNegative()) {
        if (getIntLen() > n.getIntLen()) {
            return true;
        } else if (getIntLen() < n.getIntLen()) {
            return false;
        } else {
            const auto len0 = m_number.size();
            const auto len1 = n.m_number.size();
            const auto lenMax = len0 > len1 ? len0 : len1;
            const auto lenMin = len0 < len1 ? len0 : len1;
            bool isThisBigger = false;
            for (lenType i = 0; i < lenMax; ++i) {
                if (getIntLen() == i) continue;
                if (i < lenMin) {
                    if (c2Num(m_number[i]) == c2Num(n.m_number[i])) continue;
                    isThisBigger = c2Num(m_number[i]) > c2Num(n.m_number[i]);
                    break;
                } else {
                    isThisBigger =  len0 > len1;
                    break;
                }
            }
            return isThisBigger;
        }
    } else if (!isNegative() && n.isNegative()) {
        return true;
    } else if (isNegative() && !n.isNegative()) {
        return false;
    } else {
        return abs(*this) < abs(n);
    }
}

bool bigNumber::operator==(const bigNumber &n) const {
    return m_number == n.m_number;
}

bool bigNumber::operator!=(const bigNumber &n) const {
    return !(*this == n);
}

bool bigNumber::operator<=(const bigNumber &n) const {
    return !(*this > n);
}

bool bigNumber::operator>=(const bigNumber &n) const {
    return !(*this < n);
}

int bigNumber::operator[](const lenType index) const {
    if ('-' == m_number[index]) {
        std::cerr << "This char can't convert to a number!" << std::endl;
    }
    return c2Num(m_number[index]);
}

void bigNumber::print() const {
    std::cout << "number: " << m_number << ", decimalLen: " << m_decimalLen << std::endl;
}

bigNumber bigNumber::positiveIntegerAdd(const bigNumber &n0, const bigNumber &n1) const {
    bigNumber res;
    // Both n0 and n1 are positive, and n0 is not short then n1.
    const auto len0 = n0.m_number.size();
    const auto len1 = n1.m_number.size();
    int carry = 0;
    int singleSum = 0;
    for (std::string::size_type i = 0; i < len0; ++i) {
        if (i < len1) {
            singleSum = carry + c2Num(n0.m_number[len0 - i - 1]) + c2Num(n1.m_number[len1 - i - 1]);
        } else {
            singleSum = carry + c2Num(n0.m_number[len0 - i - 1]);
        }
        res.m_number.push_back(n2Char(singleSum % 10));
        carry = singleSum / 10;
    }
    if (carry != 0) res.m_number.push_back(n2Char(carry));
    std::reverse(res.m_number.begin(), res.m_number.end());
    return res;
}

bigNumber bigNumber::positiveIntegerSubtract(const bigNumber &n0, const bigNumber &n1) const {
    bigNumber res;
    // Both n0 and n1 are positive, and n0 is not short then n1.
    const auto len0 = n0.m_number.size();
    const auto len1 = n1.m_number.size();
    int borrow = 0;
    int singleDiff = 0;
    for(std::string::size_type i = 0; i < len0; ++i) {
        if (i < len1) {
            singleDiff = c2Num(n0.m_number[len0 - i - 1]) - borrow - c2Num(n1.m_number[len1 - i - 1]);
        } else {
            singleDiff = c2Num(n0.m_number[len0 - i - 1]) - borrow;
        }
        borrow = (singleDiff < 0 ? 1 : 0);
        singleDiff += (singleDiff < 0 ? 10 : 0);
        res.m_number.push_back(n2Char(singleDiff));
    }
    std::reverse(res.m_number.begin(), res.m_number.end());
    return res;
}

bigNumber bigNumber::positiveIntegerMultiply(bigNumber num, int n) const {
    if (ZERO == num || 0 == n) return ZERO;
    if (1 == n) return num;

    int singleRes = 0, carry = 0;
    bigNumber res;
    for (auto i = num.m_number.size(); i > 0; --i) {
        singleRes = c2Num(num.m_number[i - 1]) * n + carry;
        res.m_number.push_back(n2Char(singleRes % 10));
        carry = singleRes / 10;
    }
    if (carry != 0) res.m_number.push_back(n2Char(carry));
    std::reverse(res.m_number.begin(), res.m_number.end());
    return res;
}

bigNumber bigNumber::karatsuba(const bigNumber &n0, const bigNumber &n1) const {
    if (1 == n0.m_number.size() || 1 == n1.m_number.size()) {
        const bool isFirstSingle = (n0.m_number.size() == 1);
        return positiveIntegerMultiply(isFirstSingle ? n1 : n0, isFirstSingle ? c2Num(n0.m_number[0]) : c2Num(n1.m_number[0]));
    }

    const auto len0 = n0.m_number.size();
    const auto len1 = n1.m_number.size();
    const auto len = (len0 < len1 ? len0 : len1) / 2;
    const auto str0 = n0.m_number;
    const auto str1 = n1.m_number;
    bigNumber a0, a1, b0, b1;
    a0.m_number = str0.substr(0, len0 - len);
    a1.m_number = str0.substr(len0 - len);
    b0.m_number = str1.substr(0, len1 - len);
    b1.m_number = str1.substr(len1 - len);

    auto z0 = karatsuba(a0, b0);
    const auto z1 = karatsuba(a1, b1);
    auto z2 = karatsuba(a0 + a1, b0 + b1) - z0 - z1;
    for (lenType i = 0; i < len; ++i) {
        z0.m_number.push_back('0');
        z0.m_number.push_back('0');
        z2.m_number.push_back('0');
    }
    return z0 + z2 + z1;
}

bigNumber bigNumber::modulo(const bigNumber &dividend, const bigNumber &divisor, bigNumber &remainder) const {
    // dividend > 0 && divisor > 0 && divisor != 1, and both are integer
    if (dividend < divisor) {
        remainder = dividend;
        return ZERO;
    }

    if (dividend == divisor) {
        remainder = ZERO;
        return ONE;
    }

    bigNumber res("0");
    bigNumber dvd = dividend;
    while(dvd >= divisor) {
        bigNumber tmp("1");
        bigNumber dvr = divisor;
        while(dvd >= positiveIntegerMultiply(dvr, 2)) {
            dvr = positiveIntegerMultiply(dvr, 2);
            tmp = positiveIntegerMultiply(tmp, 2);
        }
        dvd = dvd - dvr;
        res = res + tmp;
    }
    remainder = dvd;
    return res;
}

bigNumber bigNumber::div(const bigNumber &dividend, const bigNumber &divisor, lenType precision) const {
    // dividend > 0 && divisor > 0 && divisor != 1, and both are integer
    bigNumber remainder;
    auto quotient = modulo(dividend, divisor, remainder);
    if (ZERO == remainder) return quotient;
    for (lenType i = 0; i <= precision; ++i) {
        remainder.m_number.push_back('0');
        const auto dvd = remainder;
        const char singleQuotient = modulo(dvd, divisor, remainder).m_number[0];
        if (precision == i) {
            if (c2Num(singleQuotient) >= 5)
                quotient = positiveIntegerAdd(quotient, ONE);
        } else {
            quotient.m_number.push_back(singleQuotient);
        }
        if(ZERO == remainder) break;
    }
    return quotient;
}

bigNumber bigNumber::toPositiveInteger() const {
    if (0 == m_decimalLen) return *this;
    auto res = *this;
    res.m_number.erase(res.m_number.size() - res.m_decimalLen - 1, 1);

    if ('0' == res.m_number[0]) {
        lenType i = 0;
        for( ; i < res.m_number.size() - 1 && res.m_number[i] == '0'; ++i) ;
        res.m_number = res.m_number.substr(i);
    }

    res.m_decimalLen = 0;
    return res;
}

bool bigNumber::isNegative() const {
    return '-' == m_number[0];
}

void bigNumber::fixNumber() {
    if (m_number.size() <= 1) {
        m_decimalLen = 0;
        return;
    }
    lenType pointPos = m_number.size();
    bool headTestFinished = false;
    bool tailTestFinished = false;
    lenType l = 0, r = m_number.size() - 1;
    while (!tailTestFinished || !headTestFinished) {
        if (l >= r) headTestFinished = tailTestFinished = true;
        if (!headTestFinished) {
            if (m_number[l] != '-' && m_number[l] != '0') {
                if (m_number.size() == pointPos && '.' == m_number[l]) pointPos = l;
                headTestFinished = true;
            } else {
                ++l;
            }
        }

        if (!tailTestFinished) {
            if ('0' != m_number[r]) {
                if (m_number.size() == pointPos && '.' == m_number[r]) pointPos = r;
                tailTestFinished = true;
            } else {
                --r;
            }
        }
    }

    if ('0' == m_number[l] || ('.' == m_number[l] && '.' == m_number[r])) {
        m_number = "0";
        m_decimalLen = 0;
    } else {
        if (l < r && m_number.size() == pointPos) {
            lenType lt = l, rt = r;
            while(lt < rt) {
                if ('.' == m_number[++lt] || '.' == m_number[--rt]) {
                    pointPos = ((lt - l > r - rt) ? lt : rt);
                    break;
                }
            }
        }
        m_decimalLen = m_number.size() == pointPos ? 0 : (r - pointPos);
        l = '.' == m_number[l] ? (l - 1) : l;
        r = '.' == m_number[r] ? (r - 1) : r;
        m_number = ('-' == m_number[0] ? "-" : "")
                    + m_number.substr(l, m_number.size() == pointPos ? std::string::npos : (r - l + 1));
    }
}

bigNumber::lenType bigNumber::getIntLen() const {
        return m_number.size() - m_decimalLen - (0 == m_decimalLen ? 0 : 1) - (isNegative() ? 1 : 0);
}

bigNumber abs(const bigNumber &n) {
    if (!n.isNegative()) return n;
    return negation(n);
}

bigNumber negation(const bigNumber &n) {
    auto res  = n;
    res.m_number = n.isNegative() ? res.m_number.substr(1) : ("-" + res.m_number);
    return res;
}

void alignment(bigNumber &n0, bigNumber &n1) {
    const auto dLen0 = n0.m_decimalLen;
    const auto dLen1 = n1.m_decimalLen;
    if (0 == dLen0 && 1 == dLen1) return;

    const auto decimalLenDiff = dLen0 > dLen1 ? dLen0 - dLen1 : dLen1 - dLen0;
    if (dLen0 != 0) {
        n0.m_number.erase(n0.m_number.size() - dLen0 - 1, 1);
        if ('0' == n0.m_number[0]) {
            bigNumber::lenType i = 0;
            for( ; i < n0.m_number.size() - 1 && n0.m_number[i] == '0'; ++i) ;
            n0.m_number = n0.m_number.substr(i);
        }
        n0.m_decimalLen = 0;
    }
    if (dLen1 != 0) {
        n1.m_number.erase(n1.m_number.size() - dLen1 - 1, 1);
        if ('0' == n1.m_number[0]) {
            bigNumber::lenType i = 0;
            for( ; i < n1.m_number.size() - 1 && n1.m_number[i] == '0'; ++i) ;
            n1.m_number = n1.m_number.substr(i);
        }
        n1.m_decimalLen = 0;
    }

    if (dLen0 > dLen1) {
        for (bigNumber::lenType i = 0; i < decimalLenDiff; ++i) {
            n1.m_number.push_back('0');
        }
    } else if (dLen0 < dLen1) {
        for (bigNumber::lenType i = 0; i < decimalLenDiff; ++i) {
            n0.m_number.push_back('0');
        }
    }
}

} /* namespace BigNumber */
