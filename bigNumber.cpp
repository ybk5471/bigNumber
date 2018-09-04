#include <iostream>
#include <algorithm>

#include "bigNumber.h"

namespace BigNumber {

bigNumber::bigNumber() : m_number(""), m_decimalLen(0) {}

bigNumber::bigNumber(const long long &number) : m_number(std::to_string(number)) {
    fixNumber();
}

bigNumber::bigNumber(const std::string &numberStr) : m_number(numberStr) {
    fixNumber();
}

bigNumber::~bigNumber() {}

bigNumber bigNumber::operator+(const bigNumber &n) const  {
    const auto dLen0 = m_decimalLen;
    const auto dLen1 = n.m_decimalLen;

    bigNumber num0 = abs(*this), num1 = abs(n);
    alignment(num0, num1);
    const auto &numMax = num0 > num1 ? num0 : num1;
    const auto &numMin = num0 < num1 ? num0 : num1;
    bigNumber res;
    bool resultIsNegative = false;

    if (isNegative() == n.isNegative()) {
        resultIsNegative = isNegative();
        res = positiveIntegerAdd(numMax, numMin);
    } else {
        resultIsNegative = abs(*this) > abs(n) ? isNegative() : n.isNegative();
        res = positiveIntegerSubtract(numMax, numMin);
    }
    if (0 != dLen0 || 0 != dLen1) {
        res.m_number.insert(res.m_number.size() - (dLen0 > dLen1 ? dLen0 : dLen1), 1, '.');
    }
    if (resultIsNegative) {
        res.m_number = "-" + res.m_number;
    }
    res.fixNumber();
    return res;
}

bigNumber bigNumber::operator-(const bigNumber &n) const {
    const auto dLen0 = m_decimalLen;
    const auto dLen1 = n.m_decimalLen;

    bigNumber num0 = abs(*this), num1 = abs(n);
    alignment(num0, num1);
    const auto &numMax = num0 > num1 ? num0 : num1;
    const auto &numMin = num0 < num1 ? num0 : num1;
    bigNumber res;
    bool resultIsNegative = false;

    if (isNegative() == n.isNegative()) {
        resultIsNegative = (isNegative() == (abs(*this) > abs(n)));
        res = positiveIntegerSubtract(numMax, numMin);
    } else {
        resultIsNegative = isNegative();
        res = positiveIntegerAdd(numMax, numMin);
    }
    if (0 != dLen0 || 0 != dLen1) {
        res.m_number.insert(res.m_number.size() - (dLen0 > dLen1 ? dLen0 : dLen1), 1, '.');
    }
    if (resultIsNegative) {
        res.m_number = "-" + res.m_number;
    }
    res.fixNumber();
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
    std::cout << m_number << std::endl;
}

bigNumber bigNumber::positiveIntegerAdd(const bigNumber &n0, const bigNumber &n1) const {
    std::string resStr;
    // len0应该不小于len1
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
        resStr.push_back(n2Char(singleSum % 10));
        carry = singleSum / 10;
    }
    if (0 != carry) resStr.push_back(n2Char(carry));
    std::reverse(resStr.begin(), resStr.end());
    return bigNumber(resStr);
}

bigNumber bigNumber::positiveIntegerSubtract(const bigNumber &n0, const bigNumber &n1) const {
    std::string resStr;
    // len0应该不小于len1，因为n0应该大于n1
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
        resStr.push_back(n2Char(singleDiff));
    }
    std::reverse(resStr.begin(), resStr.end());
    return bigNumber(resStr);
}

bool bigNumber::isNegative() const {
    return '-' == m_number[0];
}

void bigNumber::fixNumber() {
    lenType headZeroCount = 0;
    lenType tailZeroCount = 0;
    bool tailAlwaysZero = true;
    bool headTestFinished = false;
    bool tailTestFinished = false;
    lenType l = 0, r = m_number.size() - 1;
    while (!tailTestFinished || !headTestFinished) {
        if (!headTestFinished) {
            if (m_number[l] != '-' && m_number[l] != '0') {
                headTestFinished = true;
            } else {
                if ('0' == m_number[l]) ++headZeroCount;
                ++l;
            }
        }

        if (!tailTestFinished) {
            if ('.' == m_number[r]) {
                tailTestFinished = true;
            } else {
                if ('0' != m_number[r]) {
                    tailAlwaysZero = false;
                } else {
                    if (tailAlwaysZero) ++tailZeroCount;
                }
                --r;
            }
        }

        if (l >= r) headTestFinished = tailTestFinished = true;
    }
    const lenType offset0 = ('-' == m_number[0]) ? 1 : 0;
    if (('0' == m_number[l] && '0' == m_number[r])
        || ('.' == m_number[l] && (headZeroCount + tailZeroCount == m_number.size() - offset0 - 1))) {
        m_number = "0";
        m_decimalLen = 0;
    } else {
        m_decimalLen = (m_number[r] == '.' ? (m_number.size() - 1 - r - tailZeroCount) : 0);
        const lenType offset1 = ('.' == m_number[l]) ? 1 : 0;
        const lenType offset2 = ('.' == m_number[r] && m_number.size() - 1 - r == tailZeroCount) ? 1 : 0;
        const lenType offset3 = (m_number[r] != '.') ? tailZeroCount : 0;
        m_number = ('-' == m_number[0] ? "-" : "") + m_number.substr(headZeroCount + offset0 - offset1
                                                                        , m_number.size() - tailZeroCount - headZeroCount - offset0 + offset1 - offset2 + offset3);
    }
}

bigNumber::lenType bigNumber::getIntLen() const {
        return m_number.size() - m_decimalLen - (0 == m_decimalLen ? 0 : 1) - (isNegative() ? 1 : 0);
}

bigNumber abs(const bigNumber &n) {
    if (!n.isNegative()) return n;
    std::string resStr = n.m_number;
    return bigNumber(resStr.substr(1));
}

void alignment(bigNumber &n0, bigNumber &n1) {
    const auto dLen0 = n0.m_decimalLen;
    const auto dLen1 = n1.m_decimalLen;
    if (0 == dLen0 && 1 == dLen1) return;

    const auto decimalLenDiff = dLen0 > dLen1 ? dLen0 - dLen1 : dLen1 - dLen0;
    if (0 != dLen0) {
        n0.m_number.erase(n0.m_number.size() - dLen0 - 1, 1);
    }
    if (0 != dLen1) {
        n1.m_number.erase(n1.m_number.size() - dLen1 - 1, 1);
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
