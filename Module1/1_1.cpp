/*
Даны два массива целых чисел одинаковой длины A[0..n-1] и B[0..n-1].
Необходимо найти первую пару индексов i0 и j0, i0 ≤ j0,
такую что A[i0] + B[j0] = max {A[i] + B[j], где 0 <= i < n, 0 <= j < n, i <= j}. 
Требования: Время работы - O(n).
n ≤ 100000.
*/

#include <iostream>
#include <cassert>
#include <sstream>

void run(std::istream &input, std::ostream &output)
{
    long n = 0;
    input >> n;
    assert(n > 0);

    long *A = new long[n];
    long *B = new long[n];
    for (long iii = 0; iii < n; iii++)
        input >> A[iii];
    for (long iii = 0; iii < n; iii++)
        input >> B[iii];

    long i0 = 0, j0 = 0, real_max_A = 0;

    for (long index = 1; index < n; index++) {
        // Check B first
        if ( B[index] > B[j0] ) {
            j0 = index;
        }
        if ( A[index] > A[i0] && A[index] > A[real_max_A] ) {
            real_max_A = index;
        }
        // Check the real_max_A then (if it differentiates)
        if ( real_max_A != i0 ) {
            if ( A[real_max_A] + B[index] > A[i0] + B[j0] ) {
                i0 = real_max_A;
                j0 = index;
            }
        }
        if ( A[index] + B[index] > A[i0] + B[j0] ) {
            i0 = index;
            j0 = index;
        }
    }

    output << i0 << " " << j0;

    delete[] A;
    delete[] B;
}

void testLogic()
{
    // пример из условия задачи
    {
        std::stringstream input;
        std::stringstream output;

        input << "4" << std::endl;
        input << "4 -8 6 0" << std::endl;
        input << "-10 3 1 1" << std::endl;
        run(input, output);

        assert(output.str() == "0 1");
    }

    {
        std::stringstream input;
        std::stringstream output;

        input << "6" << std::endl;
        input << "4 -8 6 0 10 7" << std::endl;
        input << "-10 3 1 2 1 3" << std::endl;
        run(input, output);

        assert(output.str() == "4 5");
    }

    {
        std::stringstream input;
        std::stringstream output;

        input << "6" << std::endl;
        input << "10 -5 0 3 15 5" << std::endl;
        input << "10 7 10 2 2 10" << std::endl;
        run(input, output);

        assert(output.str() == "4 5");
    }

    {
        std::stringstream input;
        std::stringstream output;

        input << "10" << std::endl;
        input << "5 -10 7 15 -8 22 0 15 21 0" << std::endl;
        input << "0 22 1 25 12 8 7 8 19 1" << std::endl;
        run(input, output);

        assert(output.str() == "5 8");
    }

    // проверяем граничные условия (в плюс)
    {
        std::stringstream input;
        std::stringstream output;

        long count = 100000;
        input << count << std::endl;

        for (int i = 0; i < count; i++)
            input << " " << i;
        input << std::endl;

        for (int i = 0; i < count; i++)
            input << " " << i;
        input << std::endl;

        run(input, output);

        assert(output.str() == "99999 99999");
    }

    // проверяем граничные условия (в минус)
    {
        std::stringstream input;
        std::stringstream output;

        long count = 100000;
        input << count << std::endl;

        for (int i = 0; i > count * -1; i--)
            input << " " << i;
        input << std::endl;

        for (int i = 0; i > count * -1; i--)
            input << " " << i;
        input << std::endl;

        run(input, output);

        assert(output.str() == "0 0");
    }

    // Проверяем пустой ввод (работает assert)
    /*
    {
        std::stringstream input;
        std::stringstream output;

        input << "";

        run(input, output);

        assert(output.str() == "0 0");
    }
    */
    std::cout << "OK" << std::endl;
}

int main() {

    testLogic();

    // run(std::cin, std::cout);
    return 0;
}