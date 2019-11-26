#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>


#include "Huffman.h"

#define ORIGINAL_FILE "/files/original_file"
#define COMPRESSED_FILE "/files/compressed_file"
#define DECOMPRESSED_FILE "/files/decompressed_file"

int main() {
    /// Check for files existence
    if ( (access(ORIGINAL_FILE, R_OK) & access(COMPRESSED_FILE, R_OK) & access(DECOMPRESSED_FILE, R_OK)) == -1 ) {
        fprintf(stderr, "Failed to open files");
        return EXIT_FAILURE;
    }

    /// Read from original file
    std::ifstream inf;
    inf.open(ORIGINAL_FILE, std::ifstream::out);
    std::vector<byte> vec;                  // stores chars from original file
    VectorOutputStream vec_output( vec );

    /// Read char by char into vector
    char temp_char;
    byte temp_byte;
    while ( !inf.get(temp_char) ) {
        temp_byte = byte(temp_char);
        vec_output.Write( temp_byte );
    }
    inf.close();

    /// Do main work here
    VectorInputStream vec_input( vec );
    Encode(vec_input, vec_output);

    /// Write result of compression to its file
    std::ofstream outf;
    outf.open(COMPRESSED_FILE, std::ofstream::in);

    while ( vec_input.Read(temp_byte) ) {
        outf << temp_byte;
    }
    outf.close();
}