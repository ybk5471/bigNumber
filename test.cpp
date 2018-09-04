#include <iostream>
#include <cassert>

#include "bigNumber.h"

int main() {
    BigNumber::bigNumber num0("111.22");
    BigNumber::bigNumber num1("0333");
    BigNumber::bigNumber num2("-065.873");
    BigNumber::bigNumber num3("-0654");

    assert(num0 == num0);
    assert(num0 < num1);
    assert(num2 > num3);
    assert(num1 <= abs(num3));
    assert(num0 >= num2);

    assert(BigNumber::bigNumber("444.22") == num0 + num1);
    assert(BigNumber::bigNumber("45.347") == num0 + num2);
    assert(BigNumber::bigNumber("-719.873") == num2 + num3);

    assert(BigNumber::bigNumber("-221.78") == num0 - num1);
    assert(BigNumber::bigNumber("221.78") == num1 - num0);
    assert(BigNumber::bigNumber("177.093") == num0 - num2);
    assert(BigNumber::bigNumber("-177.093") == num2 - num0);
    assert(BigNumber::bigNumber("588.127") == num2 - num3);
    assert(BigNumber::bigNumber("-588.127") == num3 - num2);

    std::cout << "The test completed successfully!" << std::endl;

    return 0;
}
