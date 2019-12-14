/*
 * 1.   Получаем полный граф с помощью функции Бокса-Мюллера
 *      u,v = rand() * (1.0 / RAND_MAX)
 *      X = cos(2 * pi * u) * sqrt(-2 * ln(v))
 *      Y = sin(2 * pi * u) * sqrt(-2 * ln(v))
 * 2.   Перебором находим лучший путь - C*
 * 3.   Находим путь с помощью 2-приближенного алгоритма - C
 * 4.   Считаем приближение ro(n) по формуле из лекций
 *
 * 5.   Среднее X = 1/N * sum(ro_i) - мат ожидание
 * 6.   D = sqrt(1/N * sum(ro_i - X)^2) - дисперсия
 *
*/

#include <cstdlib> //  rand() и srand()
#include <ctime>   //  time()

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    double u = rand() * (1.0 / RAND_MAX);
    double v = rand() * (1.0 / RAND_MAX);



    return 0;
}