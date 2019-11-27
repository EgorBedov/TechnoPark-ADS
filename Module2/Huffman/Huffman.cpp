#include <cstdlib>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <stack>
#include <map>
#include <limits>

#include "Huffman.h"

static void copyStream(IInputStream &input, IOutputStream &output) {
    byte value;
    while ( input.Read(value) )
        output.Write(value);
}

void Encode(IInputStream &original, IOutputStream &compressed) {
    Huffman hf(original, compressed);

    hf.Encode();
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    Huffman hf(compressed, original);

    hf.Decode();
}