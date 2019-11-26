#include <iostream>
#include <string>
#include <sstream>

#define BITS 256

void run(std::istream &input, std::ostream &output);
void testLogic();
void testBytes();
uint64_t* LSD(uint64_t *&array, size_t length, size_t byte);

template <typename T>
void printArray(T *array, size_t length) {
    for (size_t iii = 0; iii < length; iii++) {
        std::cout << array[iii] << std::endl;
    }
}

int main() {

    // testBytes();

    testLogic();

    // run(std::cin, std::cout);

    return EXIT_SUCCESS;
}

void run(std::istream &input, std::ostream &output) {
    // Input length
    size_t length = 0;
    input >> length;
    if ( length == 0 ) {
        output << "0";
        return;
    }

    // Input array
    auto *array = new uint64_t[length];
    for ( size_t iii = 0; iii < length; iii++ ) {
        input >> array[iii];
    }

    // // Print it out ( + in bytes )
    // for (size_t iii = 0; iii < length; iii++) {
    //     std::cout << array[iii] << "\t";
    //     for (size_t jjj = 0; jjj < sizeof(uint64_t); jjj++) {
    //         int bytik = (array[iii] >> ((56 - (jjj * 8)))) & 0xFF;
    //         std::cout << bytik << " ";
    //     }
    //     std::cout << std::endl;
    // }

    /// Sorting out
    for (size_t iii = 0; iii < sizeof(uint64_t); iii++) {
        LSD(array, length, iii);
    }

    for (int iii = 0; iii < length; iii++) {
        output << array[iii] << " ";
    }
    delete[] array;
}

// there is something wrong with array[iii] >> (8 * byte) & 0xFF
uint64_t* LSD(uint64_t *&array, size_t length, size_t byte) {

    int counts[BITS] {0};   // () - sets all elements to 0
    int places[BITS] {0};   // () - sets all elements to 0
    auto *new_array = new uint64_t[length];

    // Counting bytes
    for (size_t iii = 0; iii < length; iii++) {
        int step = 8 * byte;
        uint64_t element = array[iii];
        int counts_index = (element >> step) & 0xFF;
        counts[ counts_index ]++;
    }

    // Determine bytes places
    for (size_t iii = 1; iii < BITS; iii++) {       // places[0] = 0 because we store beginning indices
        int new_place = places[iii];
        places[iii] += ( places[iii-1] + counts[iii-1] );
    }

    // Rearranging array
    for (size_t iii = 0; iii < length; iii++) {
        int step = 8 * byte;
        uint64_t element = array[iii];
        int in_places_index = (element >> step) & 0xFF;
        uint64_t new_array_index = places[in_places_index]++;
        new_array[ new_array_index ] = array[iii];
    }

    // Changing original array
    delete[] array;
    array = new_array;
}

void testLogic() {

    // Первое условие из задачи
    {
        std::stringstream input;
        std::stringstream output;

        input << "3" << std::endl;
        input << "4 1000000 7" << std::endl;

        run(input, output);
        assert(output.str() == "4 7 1000000 ");
    }

    // Тест свой
    {
        std::stringstream input;
        std::stringstream output;

        input << "6" << std::endl;
        input << "9223372036854775807" << std::endl;
        input << "0" << std::endl;
        input << "3689348814741910322" << std::endl;
        input << "7378697629483820644" << std::endl;
        input << "5534023222112865483" << std::endl;
        input << "1844674407370955161" << std::endl;

        run(input, output);
        assert(output.str() == "0 1844674407370955161 3689348814741910322 5534023222112865483 7378697629483820644 9223372036854775807 ");
    }

    //  Проверка на 0
    {
        std::stringstream input;
        std::stringstream output;

        input << "0" << std::endl;

        run(input, output);
        assert(output.str() == "0");
    }
    std::cout << "OK" << std::endl;
}

void testBytes() {
    uint64_t first = 9223372036854775807;

    for (size_t iii = 0; iii < sizeof(uint64_t); iii++) {
        int bytik = (first >> ((56 - (iii * 8)))) & 0xFF;
        std::cout << bytik << " ";
    }
    std::cout << "\n" << ((first >> 56) & 0xFF);
}