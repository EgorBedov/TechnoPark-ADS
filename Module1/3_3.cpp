/*
Во всех задачах из следующего списка следует написать структуру данных,
обрабатывающую команды push* и pop*.

Формат входных данных.
В первой строке количество команд n. n ≤ 1000000.
Каждая команда задаётся как 2 целых числа: a b.
a = 1 - push front
a = 2 - pop front
a = 3 - push back
a = 4 - pop back
Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
Если дана команда pop*, то число b - ожидаемое значение.
Если команда pop вызвана для пустой структуры данных, то ожидается “-1”. 

Формат выходных данных.
Требуется напечатать YES - если все ожидаемые значения совпали.
Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.


Реализовать очередь с помощью двух стеков.
Использовать стек, реализованный с помощью динамического буфера.
Требования: Очередь должна быть реализована в виде класса.
Стек тоже должен быть реализован в виде класса.
*/

#include <iostream>
#include <sstream>
#include <cassert>

class DynStack {
private:
    int* buffer;    // Содержимое
    int buffer_size; // размер буфера
    int real_size;   // Он же хэд
public:
    DynStack() {
        buffer = new int[10];
        buffer_size = 10;
        real_size = 0;
    }
    ~DynStack() {
        delete[] buffer;
    }

    bool IsEmpty();
    void Expand();
    void Add(int value);
    int  Pop();
};

class Queue {
private:
    DynStack left;  // Enqueue in here
    DynStack right; // Dequeue from here
public:
    bool IsEmpty();
    void Enqueue(int value);
    int  Dequeue();
};



void run(std::istream &input, std::ostream &output) {
    Queue queue;

    long commands_amount = 0;
    input >> commands_amount;

    long i = 0;
    for (i = 0; i < commands_amount; i++) {
        int command = 0;
        int value = 0;
        
        input >> command >> value;      // считываем команду
        int dequeue_value = -1;         // если очередь пустая, то значение должно быть -1
        switch (command) {
        case 2:
            if (!queue.IsEmpty())
                dequeue_value = queue.Dequeue();
            if (dequeue_value != value) {
                output << "NO" << std::endl;
                return;
            }
            break;

        case 3:
            queue.Enqueue(value);
            break;

        default:
            output << "NO" << std::endl;
            return;
        }
    }
    output << "YES" << std::endl;
    return;
}

void testLogic() {
    // Первый пример из условия задачи
    {
        std::stringstream input;
        std::stringstream output;

        input << "3" << std::endl;
        input << "3 44" << std::endl;
        input << "3 50" << std::endl;
        input << "2 44" << std::endl;
        run(input, output);

        assert(output.str() == "YES\n");
    }
    // Второй пример из условия задачи
    {
        std::stringstream input;
        std::stringstream output;

        input << "2" << std::endl;
        input << "2 -1" << std::endl;
        input << "3 10" << std::endl;
        run(input, output);

        assert(output.str() == "YES\n");
    }

    // Третий пример из условия задачи
    {
        std::stringstream input;
        std::stringstream output;

        input << "2" << std::endl;
        input << "3 44" << std::endl;
        input << "2 66" << std::endl;
        run(input, output);

        assert(output.str() == "NO\n");
    }

    // Pop для пустой должен вернуть "-1"
    {
        std::stringstream input;
        std::stringstream output;

        input << "1" << std::endl;
        input << "2 -1" << std::endl;

        run(input, output);

        assert(output.str() == "YES\n");
    }

    // Максимальный push и максимальный pop
    {
        std::stringstream input;
        std::stringstream output;

        long count = 1000000;
        input << count << std::endl;
        for (int iii = 0; iii < count; iii++)
            input << "3 " << iii << std::endl;
        for (int iii = 0; iii < count; iii++)
            input << "2 " << iii << std::endl;

        run(input, output);
        
        assert(output.str() == "YES\n");
    }

    // Максимальный push + pop
    {
        std::stringstream input;
        std::stringstream output;

        long count = 1000000;
        input << count << std::endl;
        for (int iii = 0; iii < count; iii++) {
            input << "3 " << iii << std::endl;
            input << "2 " << iii << std::endl;
        }

        run(input, output);
        
        assert(output.str() == "YES\n");
    }

    std::cout << "OK" << std::endl;
}

int main() {

    //testLogic();

    run(std::cin, std::cout);

    return 0;
}


//////// Methods' definitions //////////

/*          DynStack           */

bool DynStack::IsEmpty() {
    return real_size == 0;
}

void DynStack::Expand() {
    int bigger_buffer_size = buffer_size * 2;
    int *bigger_buffer = new int[bigger_buffer_size];

    for (int iii = 0; iii < buffer_size; iii++)
        bigger_buffer[iii] = buffer[iii];

    delete[] buffer;
    buffer = bigger_buffer;
    buffer_size = bigger_buffer_size;
}

void DynStack::Add(int value) {
    if (real_size == buffer_size)
        Expand();
    assert(real_size < buffer_size && buffer != 0);
    buffer[real_size++] = value;
}

// let user check for IsEmpty()
int DynStack::Pop() {
    assert(buffer != 0);
    return buffer[--real_size];
}

/*          Queue           */

// check for emptiness in both stacks
bool Queue::IsEmpty() {
    return ( left.IsEmpty() && right.IsEmpty() );
}

void Queue::Enqueue(int value) {
    left.Add(value);
}

// let user check for IsEmpty()
int Queue::Dequeue() {
    if (right.IsEmpty()) {
        while (!left.IsEmpty()) {
            int temp = left.Pop();
            right.Add(temp);
        }
    }
    return right.Pop();
}