#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <chrono>  // for high_resolution_clock

using std::string;
using std::vector;

struct Hasher {
	long long unsigned int operator()(const string& data) const {
		long long unsigned int hash = 0;
		for (char iii : data)
			hash = hash * 127 + iii;
		return hash;
	}
};

struct Probing {
    int operator()(const int hash, int step) const {
        return (hash + step * step / 2 + step / 2);
    }
};

template<class T>
struct HashTableNode {
	T Data;

	HashTableNode() : Data("0") {};
	explicit HashTableNode(T _data) : Data(_data) {};
};

template <class T, class Hasher, class Probing>
class HashTable {
public:
    explicit HashTable(const Hasher& _hasher, const Probing& _probing) :
        hasher_(_hasher),
        prober_(_probing),
        table_(8),
        keys_counter_(0)
        {}
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;
	~HashTable() = default;

    bool Insert(const T& data) {
        if (keys_counter_ >= 3 * table_.capacity() / 4)
            ReHash();
        int hash = hasher_(data);
        int index = hash % table_.capacity();
        T node_data = table_[index].Data;

        for ( size_t iii = 0; iii < table_.capacity(); ++iii ) {
            if ( node_data == "0" ) break;

            if ( node_data == data && node_data != "DELETED" ) return false;

            index = prober_(hash, iii + 2) % table_.capacity();
            node_data = table_[index].Data;
        }

        table_[index].Data = data;
        ++keys_counter_;
        return true;
    }

    bool Delete(const T& data) {
        int hash = hasher_(data);
        int index = hash % table_.capacity();
        T node_data = table_[index].Data;

        for ( size_t iii = 0; iii < table_.capacity(); ++iii ) {
            if ( node_data == "0" ) return false;

            if ( node_data == data && node_data != "DELETED" ) break;

            index = prober_(hash, iii + 2) % table_.capacity();
            node_data = table_[index].Data;
        }

        table_[index].Data = "DELETED";
        --keys_counter_;
        return true;
    }

    bool Find(const T& data) {
        int hash = hasher_(data);
        int index = hash % table_.capacity();

        T node_data = table_[index].Data;
        for ( size_t iii = 0; iii < table_.capacity(); ++iii ) {
            if ( node_data == "0" ) return false;

            if ( node_data == data && node_data != "DELETED" ) return true;

            index = prober_(hash, iii + 2) % table_.capacity();
            node_data = table_[index].Data;
        }
    }

private:
    void ReHash() {
        std::vector<HashTableNode<T> > new_table(table_.capacity() * 2);
        for ( size_t iii = 0; iii < table_.capacity(); ++iii ) {
            T node_data = table_[iii].Data;
            if ( node_data != "0" && node_data != "DELETED" ) {
                /// Insert new
                int new_hash = hasher_(node_data);
                int new_index = new_hash % new_table.capacity();
                T new_node_data = new_table[new_index].Data;

//                for ( size_t iii = 0; iii < table_.capacity(); ++iii ) {
//                    if ( node_data == "0" ) break;
//
//                    if ( node_data == data && node_data != "DELETED" ) return false;
//
//                    index = prober_(hash, iii + 2) % table_.capacity();
//                    node_data = table_[index].Data;
//                }

                int new_step = 2;
                while ( new_node_data != "0" && new_node_data != node_data ) {
                    new_index = prober_(new_hash, new_step++) % new_table.capacity();
                    new_node_data = new_table[new_index].Data;
                }

                new_table[new_index].Data = node_data;
                table_[iii].Data = "0";
            }
        }
        table_ = std::move(new_table);
    }

    vector<HashTableNode<T> > table_;
    int keys_counter_;
    Hasher hasher_;
    Probing prober_;
};

void run(std::istream &input, std::ostream &output);
void testLogic();
int main() {

    testLogic();
//    run(std::cin, std::cout);

    return 0;
}

                        /////////////////////////////////////////////////////////////////////
                        ///////////////////////////// TEST AREA /////////////////////////////
                        /////////////////////////////////////////////////////////////////////
void run(std::istream &input, std::ostream &output) {
    Hasher hasher_;
    Probing prober_;
    HashTable<std::string, Hasher, Probing> ht(hasher_, prober_);

    char operation;
	string word;
	while (input >> operation >> word) {
		if (operation == '+')
			output << (ht.Insert(word) ? "OK" : "FAIL") << std::endl;
		else if (operation == '?')
			output << (ht.Find(word) ? "OK" : "FAIL") << std::endl;
		else if (operation == '-')
			output << (ht.Delete(word) ? "OK" : "FAIL") << std::endl;
	}
}

void testLogic()
{
    auto start = std::chrono::high_resolution_clock::now();

    { // Условие из задачи
        std::stringstream input;
        std::stringstream output;

        input << "+ hello" << std::endl;
        input << "+ bye" << std::endl;
        input << "? bye" << std::endl;
        input << "+ bye" << std::endl;
        input << "- bye" << std::endl;
        input << "? bye" << std::endl;
        input << "? hello" << std::endl;

        run(input, output);

        assert(output.str() == "OK\nOK\nOK\nFAIL\nOK\nFAIL\nOK\n");
    }
    { // Проверяем перехеширование
        std::stringstream input;
        std::stringstream output;

        input << "+ hello\n? hello" << std::endl;
        input << "+ hola\n? hola" << std::endl;
        input << "+ hej\n? hej" << std::endl;
        input << "+ bonjour\n? bonjour" << std::endl;
        input << "+ hallo\n? hallo" << std::endl;
        input << "+ privet\n? privet" << std::endl;
        input << "+ bye\n? bye" << std::endl;
        input << "+ adios\n? adios" << std::endl;
        input << "+ hejdo\n? hejdo" << std::endl;
        input << "+ frenchbye\n? frenchbye" << std::endl;
        input << "+ tschus\n? tschus" << std::endl;
        input << "+ poka\n? poka" << std::endl;

        input << "+ poka" << std::endl;
        input << "+ bye" << std::endl;
        input << "+ frenchbye" << std::endl;

        input << "- tschus\n? tschus" << std::endl;
        input << "- adios\n? adios" << std::endl;
        input << "- hejdo\n? hejdo" << std::endl;
        input << "- frenchbye\n? frenchbye" << std::endl;
        input << "- poka\n? poka" << std::endl;
        input << "- bonjour\n? bonjour" << std::endl;
        input << "- hallo\n? hallo" << std::endl;
        input << "- privet\n? privet" << std::endl;
        input << "- bye\n? bye" << std::endl;
        input << "- hello\n? hello" << std::endl;
        input << "- hola\n? hola" << std::endl;
        input << "- hej\n? hej" << std::endl;

        input << "- hello" << std::endl;
        input << "- hallo" << std::endl;
        input << "- poka" << std::endl;

        run(input, output);

        assert(output.str() == "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\n"
                               "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\n"
                               "FAIL\nFAIL\nFAIL\n"
                               "OK\nFAIL\nOK\nFAIL\nOK\nFAIL\nOK\nFAIL\nOK\nFAIL\nOK\nFAIL\n"
                               "OK\nFAIL\nOK\nFAIL\nOK\nFAIL\nOK\nFAIL\nOK\nFAIL\nOK\nFAIL\n"
                               "FAIL\nFAIL\nFAIL\n");
    }

    { // Много элементов
        std::stringstream input;
        std::stringstream output;

        std::stringstream answer;
        size_t ELEMENTS = 10000;

        for ( size_t iii = 1; iii <= ELEMENTS; ++iii ) {
            input << "+ " << iii << "\n? " << iii << std::endl;
        }
        for ( size_t iii = ELEMENTS; iii >= 1; --iii ) {
            input << "- " << iii << "\n? " << iii << std::endl;
        }
        for ( size_t iii = 1; iii <= ELEMENTS; ++iii ) {
            answer << "OK\nOK\n";
        }
        for ( size_t iii = 1; iii <= ELEMENTS; ++iii ) {
            answer << "OK\nFAIL\n";
        }

        run(input, output);

        assert(output.str() == answer.str());
    }
    { // Хитрое условие
        std::stringstream input;
        std::stringstream output;

        input << "+ 2\n"    // OK
                 "+ 2\n"    // FAIL
                 "+ 10\n"   // OK
                 "- 2\n"    // OK
                 "- 2\n"    // FAIL
                 "+ 10\n"   // FAIL
                 "+ 2\n"    // OK
                 "+ 2\n"    // FAIL
                 "+ 10\n";  // FAIL

        run(input, output);

        assert(output.str() == "OK\nFAIL\nOK\nOK\nFAIL\nFAIL\nOK\nFAIL\nFAIL\n");
    }

//    { // Условие, если все DELETED
//        std::stringstream input;
//        std::stringstream output;
//
//        for ( size_t iii = 1; iii <= 8; ++iii ) {
//            input << "+ " << iii << "\n- " << iii << std::endl;
//        }
//        input << "? 1\n? 2\n? 3\n? 4\n? 5\n? 6\n? 7\n? 8\n";
//
//        run(input, output);
//
//        assert(output.str() == "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\n"
//                               "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\n"
//                               "FAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\n"
//                               "FAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\nFAIL\n");
//    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "OK - " << elapsed.count() << std::endl;
}