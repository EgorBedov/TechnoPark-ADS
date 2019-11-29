#include <algorithm>
#include <map>
#include <memory>
#include <vector>
#include <utility>

#include "Huffman.h"

#define ASCII_LENGTH 256
#define BITS_IN_BYTE 8
typedef unsigned char byte;

class VectorInputStream : public IInputStream {
public:
    explicit VectorInputStream() : pos(0) {}

    bool Read(byte& value) override {
        if (pos < vec.size()) {
            value = vec[pos++];
            return true;
        }
        return false;
    }
    void Write(byte value) {
        vec.push_back(value);
    }

    std::vector<byte> vec;
    int pos;
};

class VectorOutputStream : public IOutputStream {
public:
    explicit VectorOutputStream() = default;

    void Write(byte value) override {
        vec.push_back(value);
    }

    std::vector<byte> vec;
};

class Huffman;

class BitInput {
public:
    explicit BitInput(VectorInputStream &_input) : stream(_input), buf(0), bit_pos(8) {}

    bool read_bit(byte &bit) {
        if ( bit_pos == BITS_IN_BYTE ) {
            bool res = stream.Read(buf);
            bit_pos = 0;
            if ( !res )
                return false;
        }
        bit = (buf >> (BITS_IN_BYTE - 1 - (bit_pos++))) & 1;
        return true;
    }
    byte read_byte() {
        byte letter = 0;
        byte bit;
        for (size_t iii = 0; iii < BITS_IN_BYTE; ++iii) {
            read_bit(bit);
            if ( bit == 1 ) {
                letter |= 1 << (BITS_IN_BYTE - 1 - iii);
            }
        }
        return letter;
    }

    friend Huffman;
private:
    VectorInputStream &stream;
    byte buf;
    int bit_pos;
};

class BitOutput {
public:
    explicit BitOutput(IOutputStream& _stream) : stream(_stream), buf(0), bit_count(0) {}

    void write_bit(byte bit) {
        buf |= (bit & 1) << (BITS_IN_BYTE - 1 - (bit_count++));
        if ( bit_count == BITS_IN_BYTE )
            flush();
    }
    void write_char(byte letter) {
        byte bit;
        for ( int iii = BITS_IN_BYTE - 1; iii >= 0; --iii ) {
            bit = ( (letter & (1 << iii)) ? '1' : '0' ); // TODO: not sure here
            write_bit(bit);
        }
    }
    void flush() {
        stream.Write(buf);
        buf = 0;
        bit_count = 0;
    }
    friend Huffman;
private:
    IOutputStream& stream;
    byte buf;
    int bit_count;
};

class Huffman {
public:
    Huffman(VectorInputStream &_stream_in, IOutputStream& _stream_out)
      : amount_of_symbols(0),
        counter(nullptr),
        unused_bits(0),
        bit_input(_stream_in),
        bit_output(_stream_out) {}
    ~Huffman() {
        delete(counter);
    }
    struct TreeNode {
        byte letter;
        int frequency;
        std::shared_ptr<TreeNode> left;
        std::shared_ptr<TreeNode> right;

        explicit TreeNode(byte _letter,
                          int _frequency = 0,
                          std::shared_ptr<TreeNode> _left = nullptr,
                          std::shared_ptr<TreeNode> _right = nullptr)
          : letter(_letter),
            frequency(_frequency),
            left(std::move(_left)),
            right(std::move(_right)) {};

        bool operator>(const TreeNode &r) {
            return frequency > r.frequency;
        }
    };

    static bool compare_nodes(const std::shared_ptr<TreeNode> &l, const std::shared_ptr<TreeNode> &r) {
        return l > r;
    }

    void Encode() {
        AnalyzeInput();                     // Считаем кол-во, частоту символов
        BuildTree();                        // Создаём дерево
        auto node = rate_table[0];
        BuildTable(node);                   // Создаём таблицу с новыми кодами - DFS
        unused_bits = CountUnusedBits();    // Считаем кол-во неиспользованных бит
        EncodeTree(node);                   // Кодируем само дерево Хаффмана
        WritePayload();
    }
    void AnalyzeInput() {
        /// Посчитаем кол-во и частоту символов
        counter = new size_t[ASCII_LENGTH]();
        byte value;
        while ( bit_input.stream.Read(value) ) {
            if ( counter[value] == 0 ) {        // Проверяем наличие этой буквы в таблице
                amount_of_symbols++;            // Если новая буква - увеличиваем счетчик
            }
            counter[value]++;                   // Увеличиваем счётчик частоты буквы
        }

        /// Создадим таблицу частот из нод дерева
//        rate_table.reserve(amount_of_symbols);      // I wanted to use std::array tho
        for ( size_t iii = 0; iii < ASCII_LENGTH; iii++ ) {
            if ( counter[iii] != 0 ) {
                rate_table.push_back(std::make_shared<TreeNode>(
                        iii,
                        counter[iii],
                        nullptr,
                        nullptr));
            }
        }
    }
    void BuildTree() {
        /// Copy rate_table since we will need unmodified version
        // TODO: will constructor properly copy pointers?
        backup_rate_table = rate_table;

        ////// Создаём дерево //////
        // Есть два стула:
        // 1 - Sorted array -> pop last two elements -> Insertion Sort new element
        // 2 - Unsorted array -> look for two minimums -> Insert somehow
        // Делаем по первому, потому что у второго вставка будет сложная какая-то

        /// Сортируем таблицу в порядке убывания
        std::sort(rate_table.begin(), rate_table.end(), compare_nodes);

        for ( size_t iii = rate_table.size() - 1; rate_table.size() != 1; --iii ) { // Итерируем с конца
            auto temp_node = std::make_shared<TreeNode>(     // Соединяем два последних элемента в один
                    0,                                  // Новый узел не несёт в себе никакой буквы
                    rate_table[iii]->frequency + rate_table[iii - 1]->frequency,
                    rate_table[iii],                    // Левый ребёнок
                    rate_table[iii - 1]);               // Правый ребёнок
            rate_table.pop_back();
            rate_table.pop_back();

            rate_table.push_back(temp_node);

            /// Keep table sorted using Insertion Sort solely for new element
            size_t to_swap = rate_table.size() - 2; // to_swap points to the last node ( not nullptr )
            for ( ; to_swap != -1; --to_swap ) {
                if ( temp_node->frequency > rate_table[to_swap]->frequency ) {
                    rate_table[to_swap + 1].swap(rate_table[to_swap]);
                } else {
                    break;
                }
            }
        }   // теперь последний элемент в массиве и есть корень дерева
    }
    void BuildTable(const std::shared_ptr<TreeNode> &node) {
        static std::vector<bool> code;
        if ( node->left ) {
            code.push_back(0);
            BuildTable(node->left);
        }
        if ( node->right ) {
            code.push_back(1);
            BuildTable(node->right);
        }
        if ( !(node->left) && !(node->right) ) {
            new_codes.insert(std::pair<byte, std::vector<bool> >(node->letter, std::vector<bool>(code)));
        }
        if ( !code.empty() )
            code.pop_back();
    }
    int8_t CountUnusedBits() {
        int8_t bits;
        // Each letter carries itself (8 bits) and one-bit (1 bit)
        // Empty nodes are represented by ( amount_of_symbols - 1 )
        size_t tree_bits = amount_of_symbols * ( BITS_IN_BYTE + 1 ) + ( amount_of_symbols - 1 );

        // To count length of payload we need to
        // multiply frequency of each letter by bit-length of its new code
        uint64_t payload_bits = 0;
        for ( size_t iii = 0; iii < amount_of_symbols; ++iii ) {
            payload_bits += (
                    backup_rate_table[iii]->frequency
                    *
                    new_codes[backup_rate_table[iii]->letter].size() );
        }

        bits = BITS_IN_BYTE - ( tree_bits + payload_bits ) % BITS_IN_BYTE;
        /// Запишем кол-во неиспользованных бит
        bit_output.stream.Write(bits);

        return bits;
    }
    void EncodeTree(const std::shared_ptr<TreeNode> &node) {
        if ( node->left && node->right ) {
            bit_output.write_bit(0);
            EncodeTree(node->left);
            EncodeTree(node->right);
        } else {
            bit_output.write_bit(1);
            bit_output.write_char(node->letter);
        }
    }
    void WritePayload() {
        bit_input.stream.pos = 0;       // Move index of vector in its beginning
        byte value;
        while ( bit_input.stream.Read(value) ) {
            for ( size_t iii = 0; iii < new_codes[value].size(); ++iii ) {
                bit_output.write_bit(int(new_codes[value][iii]));
            }
        }
        bit_output.flush();
    }
    void Decode() {
        /// Read header
        byte bit = 0;
        bit_input.stream.Read(bit);
        unused_bits = bit;

        auto root = DecodeTree();   // Раскодировали дерево
        auto tmp = root;            // Скопировали корень

        /// Decode message
        int16_t tree_bits = amount_of_symbols * ( BITS_IN_BYTE + 1 ) + ( amount_of_symbols - 1 );
        for ( size_t iii = BITS_IN_BYTE + tree_bits; iii < bit_input.stream.vec.size() * BITS_IN_BYTE - unused_bits; ++iii ) {
            if ( !bit_input.read_bit(bit) )
                break;  // something happens at the end

            tmp = ( bit == 0 ? tmp->left : tmp->right );
            if ( !(tmp->left) && !(tmp->right) ) {
                bit_output.write_char(tmp->letter);
                tmp = root;
            }
        }
    }
    std::shared_ptr<TreeNode> DecodeTree() {
        byte bit;
        bit_input.read_bit(bit);
        if ( int(bit) == 0 ) {
            auto left_child = DecodeTree();
            auto right_child = DecodeTree();
            return std::make_shared<TreeNode>(0, 0, left_child, right_child);
        } else {
            byte letter = bit_input.read_byte();
            ++amount_of_symbols;
            return std::make_shared<TreeNode>(letter, 0, nullptr, nullptr);
        }
    }

private:
    std::map<byte, std::vector<bool> > new_codes;
    int8_t amount_of_symbols;
    int8_t unused_bits;
    std::vector<std::shared_ptr<TreeNode> > rate_table;
    std::vector<std::shared_ptr<TreeNode> > backup_rate_table;
    size_t* counter;

    BitInput bit_input;
    BitOutput bit_output;
};

static void copyStream(IInputStream &input, VectorInputStream &output) {
    byte value;
    while ( input.Read(value) )
        output.Write(value);
}

void Encode(IInputStream &original, IOutputStream &compressed) {
    VectorInputStream original_vector;
    copyStream(original, original_vector);

    Huffman hf(original_vector, compressed);
    hf.Encode();
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    VectorInputStream compressed_vector;
    copyStream(compressed, compressed_vector);

    Huffman hf(compressed_vector, original);

    hf.Decode();
}
