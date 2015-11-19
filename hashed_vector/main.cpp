#include <iostream>
#include "hashed_vector.hpp"

int32_t hash(int32_t &a) {
    return 42;
}


int main() {
    std::vector<int> hv(100);
    std::vector<int>::iterator it = hv.begin();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}