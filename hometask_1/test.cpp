#include <iostream>

#include "math_vector.hpp"


int main(int argc, char** argv) {
    MathVector a(3, -11.0, 4.0, -5.0);
    std::cout << a << std::endl;
    MathVector b = a + a;
    std::cout << b << std::endl;
    std::cin >> b;
    std::cout << b << std::endl;
    return 0;
}
