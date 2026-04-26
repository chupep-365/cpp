#include <iostream>
#include <cmath>

void linFuncRoot(double a, double b) {
    if(a == 0){
        std::cout << "not a function";
        return;
    }
    double x = -b / a;
    std::cout << a << "x + " << b << " = 0  ==>  x = " << x;
    return;
}

void sqrFuncRoots(double a, double b, double c) {
    if(a == 0){
        linFuncRoot(b, c);
        return;
    }
    double discriminant = (b * b) - (4 * a * c);
    if(discriminant >= 0) {
        double x1 = (-b - sqrt(discriminant)) / (2 * a);
        double x2 = (-b + sqrt(discriminant)) / (2 * a);
        std::cout << a << "x^2 + " << b << "x + " << c << " = 0  ==>  x1 = " << x1 << "; x2 = " << x2;
        return;
    }
    if(discriminant < 0) {
        double XrealPart = -b / (2 * a);
        double XimaginaryPart = sqrt(-discriminant) / (2 * a);
        std::cout << a << "x^2 + " << b << "x + " << c << " = 0  ==>  x1 = " << XrealPart << " + " << XimaginaryPart << "*i" << "; x2 = " << XrealPart << " - " << XimaginaryPart << "*i";
    }
    return;
}

int main() {
    std::cout << "Enter type of the function\nLinear(1)\nQuadratic(2)\n";
    int8_t funcType{};
    std::cin >> funcType;
    if(funcType == '1') {
        double coefficient1{}, coefficient2{};
        std::cout << "Enter coefficients of linear function:\n";
        std::cin >> coefficient1 >> coefficient2;
        linFuncRoot(coefficient1, coefficient2);
        return 0; 
    }
    if(funcType == '2') {
        double coefficient1{}, coefficient2{}, coefficient3{};
        std::cout << "Enter coefficients of quadratic function:\n";
        std::cin >> coefficient1 >> coefficient2 >> coefficient3;
        sqrFuncRoots(coefficient1, coefficient2, coefficient3);
        return 0;
    }
    return 0;
}