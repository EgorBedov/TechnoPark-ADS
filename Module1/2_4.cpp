/*
Дан отсортированный массив различных целых чисел A[0..n-1]
и массив целых чисел B[0..m-1]. Для каждого элемента массива B[i]
найдите минимальный индекс элемента массива A[k], ближайшего по значению к B[i].
Требования:
Время работы поиска для каждого элемента B[i]: O(log(k)).
Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон
для бинарного поиска размером порядка k, а потом уже в нем делать бинарный поиск.
*/

#include <iostream>
#include <cassert>
#include <sstream>

void findRange(long const *array, long length, const long element,
               long &begin_out, long &end_out)
{
    assert(begin_out >= 0 && end_out < length);

    while(true) {
        if (element <= array[end_out]) {
            return;
        // Проверяем, есть ли место
        // для увеличения диапазона вправо
        } else if (end_out * 2 + 1 >= length) {
            begin_out = end_out + 1;
            end_out = length - 1;
            return;
        } else {
            begin_out = end_out + 1;
            end_out = end_out * 2 + 1;
        }
    }
}

long binarySearch(long const *array, const long element,
                  long begin, long end)
{
    long first = begin;
    long last = end + 1;    // Наш end может быть = element, но last не учитывается
    while (first < last) {
        long mid = (first + last) / 2;
        if (array[mid] < element)
            first = mid + 1;
        else
            last = mid;
    }
    return first;
}

long findClosest(long const *array, const long length, const long element, long point)
{
    long result = point;
    if (point == 0)
        return result;
    // short-circuit evaluation (which is kinda scary)
    if (( point == length ) || ( element - array[point - 1] ) <= ( array[point] - element ))
        result = point - 1;
    return result;
}

void run(std::istream &input, std::ostream &output) {
    long n = 0;
    input >> n;
    assert(n > 0 && "Length of array less than 1");
    long *A = new long[n];
    for (long iii = 0; iii < n; iii++)
        input >> A[iii];

    long m = 0;
    input >> m;
    assert(m > 0 && "Length of array less than 1");
    long *B = new long[m];
    for (long iii = 0; iii < m; iii++)
        input >> B[iii];

    for (long iii = 0; iii < m; iii++) {
        long B_num = B[iii];
        long begin = 0;
        long end = 1;

        findRange(A, n, B_num, begin, end);

        long insertion_point = binarySearch(A, B_num, begin, end);

        long closest_point = insertion_point;
        if (A[insertion_point] != B_num)
            closest_point = findClosest(A, n, B_num, insertion_point);

        output << closest_point << " ";
    }
    output << "\n";

    delete[] A;
    delete[] B;
}

void testLogic() {
    // пример из условия задачи
    {
        std::stringstream input;
        std::stringstream output;

        input << "3" << std::endl;
        input << "10 20 30" << std::endl;
        input << "3" << std::endl;
        input << "9 15 35" << std::endl;
        run(input, output);

        assert(output.str() == "0 0 2 \n");
    }

    // пример из условия задачи
    {
        std::stringstream input;
        std::stringstream output;

        input << "3" << std::endl;
        input << "10 20 30" << std::endl;
        input << "4" << std::endl;
        input << "8 9 10 32" << std::endl;
        run(input, output);

        assert(output.str() == "0 0 0 2 \n");
    }

    // Пустой ввод - сработает assert line 66
}

int main() {

    // testLogic();
    // std::cout << "OK" << std::endl;
    run(std::cin, std::cout);

    return 0;
}