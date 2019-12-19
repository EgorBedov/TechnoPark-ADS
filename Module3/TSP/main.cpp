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

#define MIN_VERTICES 2
#define MAX_VERTICES 10
#define EXPERIMENTS 10


#include <cstdlib>
#include <ctime>
#include <cmath>

#include "Graph/ListGraph.h"
#include "Calculator/Calculator.h"

Vertex create_vertex(size_t _id) {
    double u = rand() * (1.0 / RAND_MAX);
    double v = rand() * (1.0 / RAND_MAX);

    return {_id,
            cos(2 * M_PI * u) * sqrt( -2 * log10(v) ),
            sin(2 * M_PI * u) * sqrt( -2 * log10(v) )};
}

int main() {
    /// Preparation
    srand(static_cast<unsigned int>(time(nullptr)));

    for ( size_t N = MIN_VERTICES; N <= MAX_VERTICES; ++N ) {
        Calculator calc(N);
        printf("Running for N = %lu\n", N);
        for ( size_t iii = 0; iii < EXPERIMENTS; ++iii ) {
            ListGraph graph(N);
            for ( size_t id = 0; id < N; ++id ) {
                graph.AddVertex(create_vertex(id));
            }
            graph.ConnectAll();
            double C = graph.EnumerateTrip();
            double C_star = graph.SpanningTree();
            calc.CalculateRo(C, C_star);
        }

        /// Printing out results
        printf("\tME = %f\n", calc.MathExpectation());
        printf("\tD = %f\n", calc.Dispersion());
        printf("\n");
    }

    return 0;
}

// TODO: Add IGraph and switch when necessary to MatrixGraph
// TODO: Add visualization
