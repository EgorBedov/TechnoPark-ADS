#ifndef HW_HUFFMAN_H
#define HW_HUFFMAN_H

typedef unsigned char byte;

class IInputStream {
public:
    virtual bool Read(byte& value) = 0;
    virtual ~IInputStream() = default;
};

class IOutputStream {
public:
    virtual void Write(byte value) = 0;
    virtual ~IOutputStream() = default;
};

#endif //HW_HUFFMAN_H
