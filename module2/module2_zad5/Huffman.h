//
// Created by Sergei Alexeev on 14.05.2020.
//

#ifndef MODULE2_ZAD5_HUFFMAN_H
#define MODULE2_ZAD5_HUFFMAN_H

#include <string>
#include <vector>

using namespace std;

typedef unsigned char byte;
#define interface struct

interface IInputStream {
    // Возвращает false, если поток закончился
    virtual bool Read(byte& value) = 0;
};

interface IOutputStream {
    virtual void Write(byte value) = 0;
};

struct VectorInputStream: public IInputStream {
    explicit VectorInputStream(const string & init) {
        for(const auto & item : init) {
            buf.push_back(item);
        }
    }
    virtual bool Read(byte& value) {
        if (c == buf.size()) {
            return false;
        }
        value = buf[c++];
        return true;
    }
    vector<byte> buf;
    size_t c = 0;
};

class VectorOutputStream: public IOutputStream {
public:
    void Write(byte value) {
        buf.push_back(value);
    }
    vector<byte> buf;
};

void Decode(IInputStream& compressed, IOutputStream& original);
void Encode(IInputStream& original, IOutputStream& compressed);

void TestDecode(IInputStream& compressed, IOutputStream& original);
void TestEncode(IInputStream& original, IOutputStream& compressed);

#endif //MODULE2_ZAD5_HUFFMAN_H
