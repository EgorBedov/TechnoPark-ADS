#include "Calculator.h"

Calculator::Calculator(size_t _size) : amount_(_size) {
    ROs.reserve(_size);
}

void Calculator::CalculateRo(double C, double C_star) {
    ROs.push_back(std::max( C/C_star, C_star/C ));
}

double Calculator::MathExpectation() {
    math_expectation_ = 1.0 / ROs.size() * std::accumulate(ROs.begin(), ROs.end(), 0.0);
    return math_expectation_;
}

double Calculator::Dispersion() {
    double tmp = 0.0;
    for ( const auto& ro : ROs ) {
        tmp += pow(ro - math_expectation_, 2);
    }
    dispersion_ = sqrt(1.0 / ROs.size() * tmp);
    return dispersion_;
}
