#include <iostream>

#include "math_vector.hpp"


int main(int argc, char** argv) {
    std::cout << "Input format: (vect) (+|-|*) (vect) or number * (vect)," << std::endl;
    std::cout << "e.g. (-13.45, 15) + (0.45, -5), -13.45 * (56, 0)" << std::endl;
    std::cout << "Enter q to exit" << std::endl;

    char c;
    MathVector a;
    MathVector b;
    double coef;

    std::cin >> c;
    while (c != 'q') {
        std::cin.putback(c);
        std::cout << ">> ";
        if ( c != '(') { 
            std::cin >> coef >> c >> a;
            std::cout << coef * a;
        } else {
            std::cin >> a >> c >> b;
            switch (c) {
                case '+':
                    std::cout << a + b;
                    break;
                case '-':
                    std::cout << a - b;
                    break;
                case '*':
                    std::cout << a * b;
                    break;
                default:
                    std::cout << "Unknown operation";
            }
        }
        std::cout << std::endl;
        std::cin >> c;
   }
}
