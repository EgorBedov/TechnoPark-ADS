#include <iostream>
#include <sstream>
#include <cassert>

template <typename T>
void swaps( T &a, T &b ) {
    T temp = b;
    b = a;
    a = temp;
}

void printArray(int *arr, int length) {
    for (int iii = 0; iii < length; iii++) {
        std::cout << arr[iii] << " ";
    }
    std::cout << std::endl;
}

template <typename T>
bool ascending(T left, T right);
template <typename T>
bool descending(T left, T right);

int Partition(int *arr, int begin, int end, bool (*comparison)(int left, int right));

void kth_element(int *arr, int length, int index) {
    int l = 0;
    int r = length;
    while (l < r) {
        int pivot_index = Partition(arr, l, r-1, ascending);
        if ( pivot_index < index ) {
            l = pivot_index + 1;
        } else {
            r = pivot_index;
        }
    }
}

void testLogic();
void run(std::istream &input, std::ostream &output);

//////////////////////////
////////// Main //////////
//////////////////////////

int main() {

    // testLogic();

    run(std::cin, std::cout);

    return 0;
}

void run(std::istream &input, std::ostream &output) {
    int length;
    int index;
    input >> length >> index;
    if ( length == 0 )
        return;

    int *array = new int[length];
    for (int iii = 0; iii < length; iii++) {
        input >> array[iii];
    }

    kth_element(&array[0], length, index);
    output << array[index] << std::endl;

    delete[] array;
}

template <typename T>
bool ascending(T left, T right) {
    return left <= right;
}
template <typename T>
bool descending(T left, T right) {
    return left >= right;
}

int Partition(int *arr, int begin, int end, bool (*comparison)(int left, int right)) {
    // Choosing pivot
    int mid = (begin + end) / 2;
    if ( arr[mid] < arr[begin] ) {
        swaps(arr[mid], arr[begin]);
    }
    if ( arr[end] < arr[begin] ) {
        swaps(arr[end], arr[begin]);
    }
    if ( arr[mid] < arr[end] ) {
        swaps(arr[end], arr[mid]);
    }
    int pivot = arr[end];

    // Sorting
    int iii = begin;
    for ( int jjj = iii; jjj < end; jjj++ ) {
        if ( comparison(arr[jjj], pivot) ) {
            swaps( arr[iii], arr[jjj] );
            iii++;
        }
    }
    swaps( arr[iii], arr[end] );

    return iii;
}

void testLogic() {
    // Первый пример из условия задачи
    {
        std::stringstream input;
        std::stringstream output;

        input << "10 4" << std::endl;
        input << "1 2 3 4 5 6 7 8 9 10" << std::endl;

        run(input, output);
        assert(output.str() == "5\n");
    }

    // Второй пример из условия задачи
    {
        std::stringstream input;
        std::stringstream output;

        input << "10 5" << std::endl;
        input << "3 6 5 7 2 9 8 10 4 1" << std::endl;

        run(input, output);
        assert(output.str() == "6\n");
    }

    // Третий пример из условия задачи
    {
        std::stringstream input;
        std::stringstream output;

        input << "10 9" << std::endl;
        input << "0 0 0 0 0 0 0 0 0 1" << std::endl;

        run(input, output);
        assert(output.str() == "1\n");
    }

    // Пробуем пустой массив
    {
        std::stringstream input;
        std::stringstream output;

        input << "0 0" << std::endl;
        input << "" << std::endl;

        run(input, output);
        assert(output.str() == "");
    }

    // Пробуем массив c 1 элементом
    {
        std::stringstream input;
        std::stringstream output;

        input << "1 0" << std::endl;
        input << "10" << std::endl;

        run(input, output);
        assert(output.str() == "10\n");
    }

    // Пробуем массив c одинаковыми элементами
    {
        std::stringstream input;
        std::stringstream output;

        input << "10 5" << std::endl;
        input << "0 0 0 0 0 0 0 0 0 0" << std::endl;

        run(input, output);
        assert(output.str() == "0\n");
    }

    // Большой массив
    {
        std::stringstream input;
        std::stringstream output;

        int count = 10000;

        input << "10000 9999" << std::endl;
        for (int iii = 0; iii < count; iii++) {
            input << iii << " ";
        }
        input << std::endl;

        run(input, output);
        assert(output.str() == "9999\n");
    }

    std::cout << "OK" << std::endl;
}