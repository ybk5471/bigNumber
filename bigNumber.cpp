#include <iostream>
#include <algorithm>

#include "bigNumber.h"

namespace BigNumber {

bigNumber::bigNumber(const long long &number) : m_number(std::to_string(number)) {
    initDecimalLen();
}

bigNumber::bigNumber(const std::string &numberStr) : m_number(numberStr) {
    initDecimalLen();
}

bigNumber::~bigNumber() {}

bigNumber bigNumber::operator+(const bigNumber &n) const  {
    //将小数转换成整数进行相加
    const auto dLen0 = m_decimalLen;
    const auto dLen1 = n.m_decimalLen;
    bigNumber num0 = *this, num1 = n;
    //去掉小数点
    if (0 != dLen0 || 0 != dLen1) {
        const auto decimalLenDiff = dLen0 > dLen1 ? dLen0 - dLen1 : dLen1 - dLen0;
        if (0 != dLen0) {
            num0.m_number.erase(num0.m_number.size() - dLen0 - 1, 1);
        }
        if (0 != dLen1) {
            num1.m_number.erase(num1.m_number.size() - dLen1 - 1, 1);
        }
        //给小数位补0,补齐位数
        if (dLen0 > dLen1) {
            for (lenType i = 0; i < decimalLenDiff; ++i) {
                num1.m_number.push_back('0');
            }
        } else if (dLen0 < dLen1) {
            for (lenType i = 0; i < decimalLenDiff; ++i) {
                num0.m_number.push_back('0');
            }
        }
    }
    auto res = positiveIntegerAdd(num0, num1);
    if (0 != dLen0 || 0 != dLen1) {
        res.m_number.insert(res.m_number.size() - (dLen0 > dLen1 ? dLen0 : dLen1), 1, '.');
    }
    return res;
}

bigNumber bigNumber::operator-(const bigNumber &n) const {
    return positiveIntegerSubtract(*this, n);
}

bool bigNumber::operator<(const bigNumber &n) const {
    if (!isNegative() && !n.isNegative()) {
        if (getIntLen() < n.getIntLen()) {
            return true;
        } else if (getIntLen() > n.getIntLen()) {
            return false;
        } else {
            auto len0 = m_number.size();
            auto len1 = n.m_number.size();
            auto lenMax = len0 > len1 ? len0 : len1;
            auto lenMin = len0 < len1 ? len0 : len1;
            bool isThisSmaller = true;
            for (lenType i = 0; i < lenMax; ++i) {
                if (i < lenMin) {
                    if (c2Num(m_number[i]) >= c2Num(n.m_number[i])) {
                        isThisSmaller =  false;
                        break;
                    }
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
        return !(abs(*this) < abs(n));
    }
}

bigNumber::lenType bigNumber::operator[](const lenType index) const {
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
//     const auto lenMax = len0 > len1 ? len0 : len1;
//     const auto lenMin = len0 < len1 ? len0 : len1;
    int carry = 0;
    int singleSum = 0;
//     for(std::string::size_type i = 0; i < lenMax; ++i) {
    for (std::string::size_type i = 0; i < len0; ++i) {
//         if (i < lenMin) {
        if (i < len1) {
            singleSum = carry + c2Num(n0.m_number[len0 - i - 1]) + c2Num(n1.m_number[len1 - i - 1]);
        } else {
//             singleSum = carry + c2Num(len0 > len1 ? n0.m_number[lenMax - i - 1] : n1.m_number[lenMax - i -1]);
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
        singleDiff += (singleDiff < 0 ? 10 : 0);
        borrow = (singleDiff < 0 ? 1 : 0);
        resStr.push_back(n2Char(singleDiff));
    }
    std::reverse(resStr.begin(), resStr.end());
    return bigNumber(resStr);
}

bool bigNumber::isNegative() const {
    return '-' == m_number[0];
}

void bigNumber::initDecimalLen() {
    lenType count = 0;
    for (auto i = m_number.size() - 1; i >= 0; --i) {
        if ('.' == m_number[i]) {
            break;
        } else {
            ++count;
        }
    }
    m_decimalLen =  count < m_number.size() ? count : 0;
}

bigNumber::lenType bigNumber::getIntLen() const {
        return m_number.size() - m_decimalLen - (0 == m_decimalLen ? 0 : 1) - (isNegative() ? 1 : 0);
}

bigNumber abs(const bigNumber &n) {
    if (!n.isNegative()) return n;
    std::string resStr = n.m_number;
    std::reverse(resStr.begin(), resStr.end());
    resStr.pop_back();
    std::reverse(resStr.begin(), resStr.end());
    return bigNumber(resStr);
}

} /* namespace BigNumber */
