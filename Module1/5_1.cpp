/*
В супермаркете решили оптимизировать показ рекламы.
Известно расписание прихода и ухода покупателей (два целых числа).
Каждому покупателю необходимо показать минимум 2 рекламы.
Рекламу можно транслировать только в целочисленные моменты времени.
Покупатель может видеть рекламу от момента прихода до момента ухода из магазина.

В каждый момент времени может показываться только одна реклама.
Считается, что реклама показывается мгновенно.
Если реклама показывается в момент ухода или прихода, то считается,
что посетитель успел её посмотреть.

Требуется определить минимальное число показов рекламы.
*/

#include <iostream>
#include <cassert>
#include <sstream>

struct Customer
{
    size_t arr;
    size_t dep;
};


bool ascending(int a, int b)  { return a < b; }
bool descending(int a, int b) { return a > b; }
bool des_dep_des_arr(Customer* first, Customer* second) {       // true = first     false = second
    if ( first->dep < second->dep )
        return true;
    else if ( first->dep == second->dep )
        return first->arr > second->arr;
    return false;
}

void merge( Customer *arr, int begin, int mid, int end,
            bool ( *comparisonFunc)(Customer *first, Customer *second) )
{
    size_t size_left  = mid - begin + 1;
    size_t size_right = end - mid;

    Customer *left  = new Customer[size_left];
    Customer *right = new Customer[size_right];

    for (size_t iii = 0; iii < size_left; iii++) {
        left[iii] = arr[begin + iii];
    }
    for (size_t iii = 0; iii < size_right; iii++) {
        right[iii] = arr[mid + iii + 1];
    }
    
    int kkk = begin;
    size_t iii = 0, jjj = 0;
    for(; iii < size_left && jjj < size_right; kkk++) {
        if( des_dep_des_arr(&left[iii], &right[jjj]) )
            arr[kkk] = left[iii++];
        else
            arr[kkk] = right[jjj++];
    }
    while (iii < size_left)
        arr[kkk++] = left[iii++];
    while (jjj < size_right)
        arr[kkk++] = right[jjj++];
    
    delete[] left;
    delete[] right;
}

void mergeSort( Customer *arr, int begin, int end,
                bool (*comparisonFunc)(Customer *first, Customer *second) ) {
    if (end - begin == 0)
        return;
    int mid = (begin + end) / 2;
    mergeSort(arr, begin, mid, comparisonFunc);
    mergeSort(arr, mid + 1, end, comparisonFunc);
    merge(arr, begin, mid, end, comparisonFunc);
}

void run(std::istream &input, std::ostream &output) {

    size_t n = 0;
    input >> n;
    if (n == 0) {
        output << n << std::endl;
        return;
    }
    Customer *customer = new Customer[n];
    for (size_t iii = 0; iii < n; iii++)
        input >> customer[iii].arr >> customer[iii].dep;

    mergeSort(customer, 0, n-1, des_dep_des_arr);

    // for (int iii = 0; iii < n; iii++) output << customer[iii].arr << " " << customer[iii].dep << std::endl;

    ////////// Main calculation //////////
    size_t result = 2;
    size_t first_ad = customer[0].dep - 1;
    size_t last_ad = customer[0].dep;

    for (size_t iii = 1; iii < n; iii++) {
        Customer cust = customer[iii];
        
        if (cust.arr > last_ad) {
            result += 2;
            first_ad = cust.dep - 1;
            last_ad = cust.dep;
        } else if (cust.arr == last_ad) {
            result++;
            first_ad = last_ad;
            last_ad = cust.dep;
        } else if (cust.arr > first_ad && cust.dep > last_ad) {
            result++;
            first_ad = last_ad;
            last_ad = cust.dep;
        }
    }

    output << result << std::endl;
    delete[] customer;
}

void testSort() {
    {   // Рядом идущих посетителей
        std::stringstream input;
        std::stringstream output;

        input << "11" << std::endl;
        input << "1 2" << std::endl;
        input << "4 5" << std::endl;
        input << "15 16" << std::endl;
        input << "12 16" << std::endl;
        input << "6 7" << std::endl;
        input << "3 5" << std::endl;
        input << "11 18" << std::endl;
        input << "10 19" << std::endl;
        input << "19 20" << std::endl;
        input << "15 18" << std::endl;
        input << "2 5" << std::endl;

        run(input, std::cout);

        /*   После mergeSort
        for (int iii = 0; iii < n; iii++) output << customer[iii].arr << " " << customer[iii].dep << std::endl;

        for (int iii = 0; iii < n; iii++) {
            for (int jjj = 1; jjj < customer[iii].arr; jjj++)
                output << " ";
            for (int jjj = customer[iii].arr; jjj <= customer[iii].dep; jjj++)
                output << "#";
            output << std::endl;
        }
        */

    }
}

void testLogic() {
    
    {   // проверяем условия из задачи
        std::stringstream input;
        std::stringstream output;

        input << "5" << std::endl;
        input << "1 10" << std::endl;
        input << "10 12" << std::endl;
        input << "1 10" << std::endl;
        input << "1 10" << std::endl;
        input << "23 24" << std::endl;

        run(input, output);

        assert(output.str() == "5\n");
    }

    {   // Проверим на одного посетителя
        std::stringstream input;
        std::stringstream output;

        input << "1" << std::endl;
        input << "1 24" << std::endl;

        run(input, output);

        assert(output.str() == "2\n");
    }

    {   // Проверим на 4 посетителей
        std::stringstream input;
        std::stringstream output;

        input << "4" << std::endl;
        input << "1 2" << std::endl;
        input << "2 3" << std::endl;
        input << "22 23" << std::endl;
        input << "23 24" << std::endl;

        run(input, output);

        assert(output.str() == "6\n");
    }

    {   // Проверим на 0 посетителей
        std::stringstream input;
        std::stringstream output;

        input << "" << std::endl;

        run(input, output);

        assert(output.str() == "0\n");
    }
    
    {   // Рядом идущих посетителей
        std::stringstream input;
        std::stringstream output;

        input << "11" << std::endl;
        input << "1 2" << std::endl;
        input << "1 3" << std::endl;
        input << "1 5" << std::endl;
        input << "2 4" << std::endl;
        input << "3 5" << std::endl;
        input << "3 4" << std::endl;
        input << "5 24" << std::endl;
        input << "5 12" << std::endl;
        input << "18 19" << std::endl;
        input << "19 23" << std::endl;
        input << "23 24" << std::endl;

        run(input, output);

        assert(output.str() == "10\n");
    }

    {   // Все три случая проверки
        std::stringstream input;
        std::stringstream output;

        input << "11" << std::endl;
        input << "1 2" << std::endl;
        input << "4 5" << std::endl;
        input << "6 7" << std::endl;
        input << "7 10" << std::endl;
        input << "10 11" << std::endl;
        input << "12 16" << std::endl;
        input << "15 16" << std::endl;
        input << "15 18" << std::endl;
        input << "11 18" << std::endl;
        input << "10 19" << std::endl;
        input << "19 20" << std::endl;

        run(input, output);

        assert(output.str() == "12\n");
    }

    std::cout << "OK" << std::endl;
}

int main() {

    // testLogic();

    // testSort();      // пральна

    run(std::cin, std::cout);

    return 0;
}