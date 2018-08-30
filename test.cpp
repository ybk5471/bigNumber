#include <iostream>

#include "bigNumber.h"

int main() {
    ///@note: add test
    BigNumber::bigNumber num0("999.94654643213");
    BigNumber::bigNumber num1("32013946.78965746221");
    BigNumber::bigNumber num3("7986");
    BigNumber::bigNumber num4("354");

    auto num = num1 + num0;
    num.print(); ///@note: result: 32014946.73620389434

    num = num3 + num0;
    num.print(); ///@note: result: 8985.94654643213

    num = num3 + num4;
    num.print(); ///@note: result: 8340

    ///@note: subtract
    num = num3 - num4;
    num.print(); ///@note: result:

    BigNumber::bigNumber num5("-777.865");
    num = BigNumber::abs(num5);
    num.print();

    std::cout << (num0 < num1) << std::endl;
    std::cout << (num0 < num3) << std::endl;
    std::cout << (num3 < num4) << std::endl;
    std::cout << (num4 < num5) << std::endl;


    return 0;
}
