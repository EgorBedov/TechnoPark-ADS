#include "Huffman.h"

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

