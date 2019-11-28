#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>

#include "Huffman.h"

#define ORIGINAL_FILE "files/original_file"
#define COMPRESSED_FILE "files/compressed_file"
#define DECOMPRESSED_FILE "files/decompressed_file"

int main() {
    /// Check for files existence
    if ( (access(ORIGINAL_FILE, R_OK) & access(COMPRESSED_FILE, R_OK) & access(DECOMPRESSED_FILE, R_OK)) == -1 ) {
        fprintf(stderr, "Failed to open files!\n");
        return EXIT_FAILURE;
    }

    ////////////////////////////////////////////////////////////
    ///////////////////////// ENCODING /////////////////////////
    ////////////////////////////////////////////////////////////

    std::ifstream inf;
    inf.open(ORIGINAL_FILE, std::ifstream::out);
    VectorOutputStream temp_vec_output;

    /// Read char by char into vector
    char temp_char;
    byte temp_byte;
    while ( inf.get(temp_char) ) {
        temp_byte = byte(temp_char);
        temp_vec_output.Write( temp_byte );  // заполняю тот вектор, который имеет метод write
    }
    inf.close();

    /// Do main work
    // vec_input пустой, а нам необходимо из него читать
    // поэтому просто поменяю местами вектора
    VectorInputStream vec_input;
    vec_input.vec = temp_vec_output.vec;    // копируем original в новый вектор
    VectorOutputStream vec_output;          // пустой вектор для записи compressed
    printf("---ENCODING---\n");
    Encode(vec_input, vec_output);

    /// Write result of compression to its file
    std::ofstream outf;
    outf.open(COMPRESSED_FILE, std::ofstream::in | std::ofstream::trunc);   // ios::binary?

    // опять же, необходимо прочитать из vec_output,
    // у которого нет Read, поэтому снова меняем их местами
    VectorInputStream temp_vec_input;
    temp_vec_input.vec = vec_output.vec;        // копируем compressed в новый вектор
    while ( temp_vec_input.Read(temp_byte) ) {
        outf << temp_byte;
    }
    outf.close();

    ////////////////////////////////////////////////////////////
    ///////////////////////// DECODING /////////////////////////
    ////////////////////////////////////////////////////////////

    inf.open(COMPRESSED_FILE, std::ifstream::out);  // ios::binary?
    temp_vec_output.vec.clear();

    /// Read char by char into vector
    while ( inf.get(temp_char) ) {
        temp_byte = byte(temp_char);
        temp_vec_output.Write( temp_byte );
    }
    inf.close();

    /// Prepare arguments for Decode()
    vec_input.vec.clear();
    vec_input.vec = temp_vec_output.vec;
    vec_input.pos = 0;
    vec_output.vec.clear();
    printf("\n\n---DECODING---\n");
    Decode(vec_input, vec_output);

    /// Write result of compression to its file
    outf.open(DECOMPRESSED_FILE, std::ofstream::in | std::ofstream::trunc | std::ofstream::binary);

    temp_vec_input.vec.clear();             // Очищаем вектор из которого будем читать
    temp_vec_input.vec = vec_output.vec;    // Копируем в него наш результат
    temp_vec_input.pos = 0;
    while ( temp_vec_input.Read(temp_byte) ) {
        outf << char(temp_byte);
    }
    outf.close();

    ///////////////////////////////////////////////////////////
    ///////////////////////// CONTROL /////////////////////////
    ///////////////////////////////////////////////////////////

    std::ifstream inf2;
    inf.open(ORIGINAL_FILE, std::ifstream::out);
    inf2.open(DECOMPRESSED_FILE, std::ifstream::out);

    /// Check lengths of files
    inf.seekg(0, inf.end);
    inf2.seekg(0, inf2.end);
    int orig_length = inf.tellg();
    int decomp_length = inf2.tellg();
    inf.seekg(0, inf.beg);
    inf2.seekg(0, inf2.beg);

    if ( orig_length != decomp_length ) {
        printf("Files aren't the same length!\n");
    } else {
        printf("Files are the same length\n");
    }
    printf("\tOriginal file: %d\n"
           "\tDecompressed file: %d\n",
           orig_length, decomp_length);

    char orig;
    char decomp;
    while ( inf.get(orig) && inf.get(decomp) ) {
        if ( orig != decomp ) {
            if ( int(inf2.tellg()) != 0 ) {
                printf("Files aren't identical!\n"
                   "\tORIG: %d - %c\n"
                   "\tDECO: %d - %c\n",
                   int(inf.tellg()), orig, int(inf2.tellg()), decomp);
                break;
            }
        }
    }

    inf.close();
    inf2.close();

}

// TODO: too many vectors
