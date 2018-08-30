#include "bigNumber.cpp"

int main() {
    BigNumber::bigNumber num1("78.999668439576");
    BigNumber::bigNumber num2("8568.789996987");
    auto num = num1 + num2;
    num.print();
    return 0;
}
8568.789996987
  78.999668439576
85.3,678554316576
