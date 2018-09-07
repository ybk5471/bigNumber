#include <iostream>
#include <cassert>

#include "bigNumber.h"

int main() {
//     fixNumber function test
//     BigNumber::bigNumber num0("-000.00000");
//     num0.print();
//     BigNumber::bigNumber num1("-000000");
//     num1.print();
//     BigNumber::bigNumber num2("-0000000");
//     num2.print();
//     BigNumber::bigNumber num3("-0.0000700");
//     num3.print();
//     BigNumber::bigNumber num4("-008090.070070000");
//     num4.print();
//     BigNumber::bigNumber num5("-00008");
//     num5.print();
//     BigNumber::bigNumber num6("-8.000");
//     num6.print();
//     BigNumber::bigNumber num7("-0");
//     num7.print();
//     BigNumber::bigNumber num8("-8");
//     num8.print();
//     BigNumber::bigNumber num9("-8000");
//     num9.print();
//     BigNumber::bigNumber num10("-080");
//     num10.print();
//     BigNumber::bigNumber num11("-0880");
//     num11.print();
//     BigNumber::bigNumber num12("-0080");
//     num12.print();
//     BigNumber::bigNumber num13("-0.800");
//     num13.print();
//     BigNumber::bigNumber num14("-0.80");
//     num14.print();
//     BigNumber::bigNumber num15("-0.8");
//     num15.print();



    BigNumber::bigNumber num0("111.22");
    BigNumber::bigNumber num1("0333");
    BigNumber::bigNumber num2("-065.873");
    BigNumber::bigNumber num3("-0654");
    BigNumber::bigNumber num4("-0.0007");
    BigNumber::bigNumber num5("7.879");
    BigNumber::bigNumber num6("111");
    BigNumber::bigNumber num7("-110");
    BigNumber::bigNumber num8("-111");

    assert(BigNumber::bigNumber("111") != BigNumber::bigNumber("-111.0"));
    assert(BigNumber::bigNumber("111") == BigNumber::abs(BigNumber::bigNumber("-111.0")));
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

    assert(BigNumber::bigNumber("37036.26") == num0 * num1);
    assert(BigNumber::bigNumber("37036.26") == num1 * num0);
    assert(BigNumber::bigNumber("-21935.709") == num1 * num2);
    assert(BigNumber::bigNumber("-21935.709") == num2 * num1);
    assert(BigNumber::bigNumber("-7326.39506") == num0 * num2);
    assert(BigNumber::bigNumber("-7326.39506") == num2 * num0);
    assert(BigNumber::bigNumber("-72737.88") == num0 * num3);
    assert(BigNumber::bigNumber("-72737.88") == num3 * num0);
    assert(BigNumber::bigNumber("43080.942") == num2 * num3);
    assert(BigNumber::bigNumber("43080.942") == num3 * num2);
    assert(BigNumber::bigNumber("-0.0055153") == num4 * num5);

    assert(BigNumber::bigNumber("-0.000088843761898718111") == num4 / num5);
    assert(BigNumber::bigNumber("0.333993993993993993994") == num0 / num1);
    assert(BigNumber::bigNumber("0.000001070336391437309") == num4 / num3);
    assert(BigNumber::bigNumber("3") == num1 / num6);
    assert(BigNumber::bigNumber("0") == num6 / num1);
    assert(BigNumber::bigNumber("14.116004569107754791217") == num0 / num5);
    assert(BigNumber::ZERO == num1 / num3);
    assert(BigNumber::ONE == num6 % num7);
    assert(BigNumber::bigNumber("-1") == num8 % num7);
    assert(num7 == num7 % num6);
    assert(BigNumber::ZERO == num1 % num6);
    assert(num6 == num6 % num1);
    assert(BigNumber::bigNumber("3") == num1 % num7);

    assert(BigNumber::bigNumber("-285839.555493617") == num2.pow(3));

    std::cout << "The test completed successfully!" << std::endl;
    return 0;
}
