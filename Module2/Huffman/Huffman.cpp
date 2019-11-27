

#include "Huffman.h"

static void copyStream(IInputStream &input, VectorInputStream &output) {
    byte value;
    while ( input.Read(value) )
        output.Write(value);
}

void Encode(IInputStream &original, IOutputStream &compressed) {
    std::vector<byte> vec;
    VectorInputStream original_vector(vec);

    copyStream(original, original_vector);

    Huffman hf(original_vector, compressed);
    hf.Encode();
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    std::vector<byte> vec;
    VectorInputStream compressed_vector(vec);

    copyStream(compressed, compressed_vector);

    Huffman hf(compressed_vector, original);

    hf.Decode();
}

