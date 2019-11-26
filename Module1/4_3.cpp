/*
Решение всех задач данного раздела предполагает использование кучи,
реализованной в виде класса. Решение должно поддерживать передачу функции сравнения снаружи.

Тупики.
На вокзале есть некоторое количество тупиков, куда прибывают электрички.
Этот вокзал является их конечной станцией. Дано расписание движения электричек,
в котором для каждой электрички указано время ее прибытия,
а также время отправления в следующий рейс.

Электрички в расписании упорядочены по времени прибытия.
Когда электричка прибывает, ее ставят в свободный тупик с минимальным номером.
При этом если электричка из какого-то тупика отправилась в момент времени X,
то электричку, которая прибывает в момент времени X, в этот тупик ставить нельзя,
а электричку, прибывающую в момент X+1 — можно.
В данный момент на вокзале достаточное количество тупиков для работы по расписанию.
Напишите программу, которая по данному расписанию определяет,
какое минимальное количество тупиков требуется для работы вокзала.

Формат входных данных. Вначале вводится n - количество электричек в расписании.
Затем вводится n строк для каждой электрички, в строке - время прибытия и время отправления.
Время - натуральное число от 0 до 10^9. Строки в расписании упорядочены по времени прибытия.
Формат выходных данных. Натуральное число - минимальное количеством тупиков.
Максимальное время: 50мс, память: 5Мб.

in          out |
=============== |
1           1   |
10 20           |
=============== |
2           2   |
10 20           |
20 25           |
=============== |
3           2   |
10 20           |
20 25           |
21 30           |


Вставляем первый элемент в топ
 Сравниваем прибытие следующего с убытием топа
    Если < то вставляем элемент в кучу //, и просеиваем вверх (топ - min dep)
    Если > то удаляем топ, заменяем его на другой элемент кучи, снова сравниваем и мб повторяем
 По итогу размер кучи и есть кол-во тупиков (std::max)
*/

#include <algorithm>
#include <iostream>
#include <cassert>
#include <sstream>

struct Train;

template <typename T>
void swaps(T &a, T &b) { T temp = b; b = a; a = temp; }

template <class T>
class DynArray;
template <class T, class Comparator = std::less<T> >
class Heap;

void run(std::istream &input, std::ostream &output);
void testLogic();

//////////////////////////
////////// Main //////////
//////////////////////////

int main() {

    testLogic();
    // run(std::cin, std::cout);

    return EXIT_SUCCESS;
}


struct Train {
    int arr;
    int dep;

    explicit Train(int _arr = 0, int _dep = 0) : arr(_arr), dep(_dep) {}

    bool operator<(const Train &r) const {
        return dep < r.dep;
    }
};

template <class T>
class DynArray {
private:
    T *buffer;      // Содержимое
    int buffer_size;    // размер буфера
    int real_size;      // Он же хэд - 1
public:
    DynArray(int n = 1) {
        buffer = new T[n];
        buffer_size = n * 2;
        real_size = 0;
    }
    ~DynArray() { delete[] buffer; }

    T& operator[] (const int index) {
        assert(index >= 0 && index <= real_size);
        return buffer[index];
    }

    bool IsEmpty()  { return real_size == 0; }
    int Size()      { return real_size; }
    T Last()    { return buffer[real_size - 1]; }

    void Grow();
    void Add(T value);
    T Pop();
    void DeleteLast() { --real_size; }
};

template <class T, class Comparator>
class Heap {
private:
    DynArray<T> array;
    Comparator comp;

    void siftDown(int index) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        int largest = index;
        if ( left < array.Size() && comp(array[left], array[index]) )
            largest = left;
        if ( right < array.Size() && comp(array[right], array[largest]) )
            largest = right;

        if ( largest != index ) {
            swaps(array[index], array[largest]);
            siftDown(largest);
        }
    }

    void siftUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if ( comp(array[parent], array[index]) )
                return;
            swaps( array[index], array[parent] );
            index = parent;
        }
    }

public:
    Heap(Comparator comp = Comparator()) : array() {};
    ~Heap() = default;

    void Insert( T element );
    T ReplaceHead();
    T PeekMax();

    bool IsEmpty() { return array.IsEmpty(); };
    int Size() { return array.Size(); };

    void PrintInfo( int index );

};

template <class T>
class Comparator {
public:
    bool operator() (const T &left, const T &right) {
        return left < right;
    }
};

/////////////////////////
////////// RUN //////////
/////////////////////////

void run(std::istream &input, std::ostream &output) {

    // Вводим кол-во поездов
    int n = 0;
    input >> n;
    if (n == 0) {
        output << n << std::endl;
        return;
    }

    // Создаём кучу и добавляем первый элемент
    Heap<Train, Comparator<Train>> trains;
    Train next;
    input >> next.arr >> next.dep;
    trains.Insert(next);
    int max = trains.Size();

    // Сортируем и считаем
    for (size_t iii = 1; iii < n; iii++) {
        input >> next.arr >> next.dep;

        Train top = trains.PeekMax();
        while ( !trains.IsEmpty() && next.arr > top.dep) {
            trains.ReplaceHead();
        }
        trains.Insert(next);

        if ( trains.Size() > max )
            max = trains.Size();
    }

    output << max << std::endl;
}

//////////////////////////
////////// Heap //////////
//////////////////////////
template <class T, class Comparator>
void Heap<T, Comparator>::Insert(T element) {
    array.Add( element );
    siftUp( array.Size() - 1);
}

template <class T, class Comparator>
T Heap<T, Comparator>::ReplaceHead() {
    assert( !array.IsEmpty() );
    // Запоминаем топ
    T result = array[0];

    array[0] = array.Last();
    array.DeleteLast();

    if ( !array.IsEmpty() )
        siftDown( 0 );

    return result;
}

template <class T, class Comparator>
T Heap<T, Comparator>::PeekMax() {
    assert ( !array.IsEmpty() );
    return array[0];
}

template <class T, class Comparator>
void Heap<T, Comparator>::PrintInfo( int index ) {
    std::cout << array[index].arr << " " << array[index].dep << std::endl;
}

//////////////////////////////
////////// DynArray //////////
//////////////////////////////

template <typename T>
void DynArray<T>::Grow()
{
    int bigger_buffer_size = buffer_size * 2;
    auto *bigger_buffer = new T[bigger_buffer_size];

    for (size_t iii = 0; iii < buffer_size; iii++)
        bigger_buffer[iii] = buffer[iii];

    delete[] buffer;
    buffer = bigger_buffer;
    buffer_size = bigger_buffer_size;
}

template <typename T>
void DynArray<T>::Add(T value) {
    if ( real_size == buffer_size ) Grow();
    assert( real_size < buffer_size && buffer != nullptr );
    buffer[real_size++] = value;
}

template <typename T>
T DynArray<T>::Pop() {
    assert(buffer != nullptr);
    return buffer[--real_size];
}

//////////////////////////
////////// Test //////////
//////////////////////////

void testLogic()
{
    { // Первое условие
        std::stringstream input;
        std::stringstream output;

        input << "1" << std::endl;
        input << "10 20" << std::endl;

        run(input, output);

        assert(output.str() == "1\n");
    }

    { // Второе условие
        std::stringstream input;
        std::stringstream output;

        input << "2" << std::endl;
        input << "10 20" << std::endl;
        input << "20 25" << std::endl;

        run(input, output);

        assert(output.str() == "2\n");
    }

    { // Третье условие
        std::stringstream input;
        std::stringstream output;

        input << "3" << std::endl;
        input << "10 20" << std::endl;
        input << "20 25" << std::endl;
        input << "21 30" << std::endl;

        run(input, output);

        assert(output.str() == "2\n");
    }

    { // Свои условия
        std::stringstream input;
        std::stringstream output;

        input << "5" << std::endl;
        input << "1 24" << std::endl;
        input << "2 17" << std::endl;
        input << "5 12" << std::endl;
        input << "8 23" << std::endl;
        input << "10 11" << std::endl;

        run(input, output);

        assert(output.str() == "5\n");
    }

    { // Нет поездов
        std::stringstream input;
        std::stringstream output;

        input << "0" << std::endl;

        run(input, output);

        assert(output.str() == "0\n");
    }

    std::cout << "OK" << std::endl;
}