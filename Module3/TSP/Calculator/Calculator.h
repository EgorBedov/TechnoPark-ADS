#ifndef HW_CALCULATOR_H
#define HW_CALCULATOR_H


#include <climits>
#include <cmath>
#include <cstdlib>
#include <numeric>
#include <queue>
#include <vector>

class Calculator {
public:
    explicit Calculator(size_t _size);

    void CalculateRo(double C, double C_star);
    double MathExpectation();
    double Dispersion();
private:
    std::vector<double> ROs;
    double math_expectation_ = 0.0;
    double dispersion_ = 0.0;

    size_t amount_;
};


#endif // HW_CALCULATOR_H
