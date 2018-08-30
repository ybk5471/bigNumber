#include <iostream>
#include <algorithm>

#include "bigNumber.h"

namespace BigNumber {

bigNumber::bigNumber(const long long &number) : m_number(std::to_string(number)) {}

bigNumber::bigNumber(const std::string &numberStr) : m_number(numberStr) {}

bigNumber::~bigNumber() {}

bigNumber bigNumber::operator+(const bigNumber &n) const  {
    //将小数转换成整数进行相加
    const auto dLen0 = getDecimalLen();
    const auto dLen1 = n.getDecimalLen();
    bigNumber num0 = *this, num1 = n;
    //去掉小数点
    if (0 != dLen0 || 0 != dLen1) {
        const auto decimalLenDiff = dLen0 > dLen1 ? dLen0 - dLen1 : dLen1 - dLen0;
        if (0 != dLen0) {
            std::string str0 = m_number;
            num0 = bigNumber(str0.erase(str0.size() - static_cast<std::string::size_type>(dLen0) - 1, 1));
        }
        if (0 != dLen1) {
            std::string str1 = n.m_number;
            num1 = bigNumber(str1.erase(str1.size() - static_cast<std::string::size_type>(dLen1) - 1, 1));
        }
        //给小数位补0,补齐位数
        if (dLen0 > dLen1) {
            for (unsigned long i = 0; i < decimalLenDiff; ++i) {
                num1.m_number.push_back('0');
            }
        } else if (dLen0 < dLen1) {
            for (unsigned long i = 0; i < decimalLenDiff; ++i) {
                num0.m_number.push_back('0');
            }
        }
    }
    auto res = __add(num0, num1);
    res.m_number.insert(res.m_number.size() - static_cast<std::string::size_type>(dLen0 > dLen1 ? dLen0 : dLen1) - 1, 1, '.');
    return res;
}

unsigned int bigNumber::operator[](const unsigned long index) const {
    if ('-' == m_number[index]) {
        std::cerr << "This char can't convert to a number!" << std::endl;
    }
    return c2Num(m_number[index]);
}

void bigNumber::print() const {
    std::cout << m_number << std::endl;
}

bigNumber bigNumber::__add(const bigNumber &n0, const bigNumber &n1) const {
    std::string resStr;
    const auto len0 = n0.m_number.size();
    const auto len1 = n1.m_number.size();
    const auto lenMax = len0 > len1 ? len0 : len1;
    const auto lenMin = len0 < len1 ? len0 : len1;
    int carry = 0;
    int singleSum = 0;
    for(std::string::size_type i = 0; i < lenMax; ++i) {
        if (i < lenMin) {
            singleSum = carry + c2Num(n0.m_number[len0 - i - 1]) + c2Num(n1.m_number[len1 - i - 1]);
        } else {
            singleSum = carry + c2Num(len0 > len1 ? n0.m_number[lenMax - i - 1] : n1.m_number[lenMax - i -1]);
        }
        resStr.push_back(n2Char(singleSum % 10));
        carry /= 10;
    }
    if (0 != carry) resStr.push_back(n2Char(carry));
    reverse(resStr.begin(), resStr.end());
    return bigNumber(resStr);
}

bool bigNumber::isNegative() const {
    return '-' == m_number[0];
}

unsigned long bigNumber::getDecimalLen() const {
    unsigned long count = 0;
    for (std::string::size_type i = 0; i < m_number.size(); ++i) {
        if ('.' != m_number[i]) {
            ++count;
        } else if ('-' == m_number[i]) {
            continue;
        } else {
            break;
        }
    }
    return static_cast<unsigned long>(m_number.size()) - count;
}

} /* namespace BigNumber */
