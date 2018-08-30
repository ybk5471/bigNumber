#include <string>

namespace BigNumber {

class bigNumber {
public:
    bigNumber(const long long &number);
    bigNumber(const std::string &numberStr);
    ~bigNumber();

    bigNumber operator+(const bigNumber &n) const;
    unsigned int operator[](const unsigned long index) const;

    void print() const;
private:
    // 两个加数要求是正整数
    bigNumber __add(const bigNumber &n0, const bigNumber &n1) const;
    bigNumber add(const long long &n) const;
    bigNumber add(const std::string &n) const;

    bool isNegative() const;
    unsigned long getDecimalLen() const;

    unsigned int c2Num(char ch) const {
        return static_cast<unsigned int>(ch - '0');
    }

    char n2Char(unsigned int i) const {
        return static_cast<char>(i + '0');
    }

private:
    std::string m_number;
};

} /* namespace BigNumber */
