#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <stack>

//#include "Huffman.h"


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
    VectorInputStream(const string & init) {
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


class BitWriter
{
public:
    BitWriter() : bitCounter(0) {}

    void WriteBit(unsigned char bit)
    {
        if (bitCounter % 8 == 0)
            buffer.push_back(0);
        if (bit)
            buffer[bitCounter/8] |= 1 << (7 - bitCounter % 8);
        bitCounter++;
    }

    void WriteByte(unsigned char byte)
    {
        if (bitCounter % 8 == 0)
        {
            buffer.push_back(byte);
        }
        else
        {
            int offset = bitCounter % 8;
            buffer[bitCounter/8] |= byte >> offset;
            buffer.push_back(byte << (8 - offset));
        }

        bitCounter += 8;
    }

    const std::vector<unsigned char> &GetBuffer() const
    {
        return buffer;
    }
    size_t GetBitCounter() const
    {
        return bitCounter;
    }
private:
    std::vector<unsigned char> buffer;
    size_t bitCounter;
};

void printBuffer(const std::vector<unsigned char> &buffer)
{
    for (auto &v: buffer)
    {
        std::cout << std::bitset<8>(v) << "|";
    }
    std::cout << std::endl;
}

class BitReader{
public:
    BitReader(const vector<byte> & init) {
        buffer = init;
    }
    int readBit() {
        byte t1 = buffer[bitCounter / 8];
        byte t2 = (1 << (7 - bitCounter % 8));
        byte t3 = t1 & t2;
        bitCounter++;
        return (t3) ? 1 : 0;
    }
    byte readByte() {
        if(bitCounter % 8 == 0) {
            int index = bitCounter / 8;
            bitCounter += 8;
            return buffer[index];
        }
        byte res;
        int offset = bitCounter % 8;
        byte t1 = buffer[bitCounter / 8];
        t1 = t1 << offset;
        byte t2 = buffer[(bitCounter / 8) + 1];
        t2 = t2 >> (8 - offset);
        bitCounter += 8;
        return t1 + t2;
    }
private:
    size_t bitCounter = 0;
    vector<byte> buffer;
};





struct Node {
    Node(byte byte = 0, int number = 0): left(nullptr), right(nullptr), symbol(byte), n(number) {}
    byte symbol;
    int n;
    Node * left, * right;
};

vector<Node *> createHeap(map<byte, int> & m) {
    vector<Node *> heap;
    for(const auto & item : m) {
        Node * node = new Node(item.first, item.second);
        heap.push_back(node);
    }
    make_heap(heap.begin(), heap.end(), [](Node * n1, Node * n2){
        return n1->n > n2->n;
    });
    return heap;
}

Node * createHaffmanTree(vector<Node *> & v) {
    while(v.size() > 1) {
        Node * node1 = v[0];
        pop_heap(v.begin(), v.end()); v.pop_back();
        Node * node2 = v[0];
        pop_heap(v.begin(), v.end()); v.pop_back();
        Node * combo = new Node();
        combo->n = node1->n + node2->n;
        combo->left = node1;
        combo->right = node2;
        v.push_back(combo);
    }
    return v[0];
}

map<byte, string> createDict(Node * tree) {
    stack<pair<Node *, string>> s;
    string startPath;
    if (tree->left == nullptr && tree->right == nullptr) {
        startPath = "1";
    }
    s.push(make_pair(tree, startPath));
    map<byte, string> dict;
    while(!s.empty()) {
        pair<Node *, string> info = s.top(); s.pop();
        if(info.first->left == nullptr && info.first->right == nullptr) {
            dict[info.first->symbol] = info.second;
            continue;
        }
        if(info.first->left) {
            s.push(make_pair(info.first->left, info.second + "0"));
        }
        if(info.first->right) {
            s.push(make_pair(info.first->right, info.second + "1"));
        }
    }
    return dict;
}

void recurseBitTree(Node * node, BitWriter & writer) {
    if (node == nullptr) {
        return;
    }
    if(node->left == nullptr && node->right == nullptr) {
        writer.WriteBit(1);
        writer.WriteByte(node->symbol);
    } else {
        recurseBitTree(node->left, writer);
        recurseBitTree(node->right, writer);
        writer.WriteBit(0);
    }
}

Node * readBitTree(int n, BitReader & reader) {
    stack<Node *> s;
    size_t counter = 0;
    while(counter < n || s.size() != 1) {
        bool bit = reader.readBit();
        if(bit) {
            byte symbol = reader.readByte();
            Node * node = new Node(symbol);
            counter++;
            s.push(node);
        } else {
            Node * n1, * n2, * combo;
            n1 = s.top(); s.pop();
            n2 = s.top(); s.pop();
            combo = new Node();
            combo->right = n1;
            combo->left = n2;
            s.push(combo);
        }
    }
    return s.top();
}


// Метод архивирует данные из потока original
void Encode(IInputStream& original, IOutputStream& compressed) {
    vector<byte> src;
    map<byte, int> counter;
    byte tmpByte;
    while(original.Read(tmpByte)) {
        src.push_back(tmpByte);
        counter[tmpByte] += 1;
    }
    if(src.empty()) { return; }
    vector<Node *> heap = createHeap(counter);
    Node * tree = createHaffmanTree(heap);
    map<byte, string> dict = createDict(tree);
    BitWriter writer;
    writer.WriteByte(dict.size());

    int sizeBit = 0;
    for(const auto & item: dict) {
        sizeBit += item.second.size() * counter[item.first];
    }
    writer.WriteByte(sizeBit);

    recurseBitTree(tree, writer);

    for(const auto & item : src) {
        string path = dict[item];
        for(const auto & bitChar: path) {
            if(bitChar == '1') {
                writer.WriteBit(1);
            } else {
                writer.WriteBit(0);
            }
        }
    }
    // удалить дерево из памяти
    printBuffer(writer.GetBuffer());
    for(const auto & item: writer.GetBuffer()) {
        compressed.Write(item);
    }
}
// Метод восстанавливает оригинальные данные
void Decode(IInputStream& compressed, IOutputStream& original) {
    vector<byte> src;
    byte tmpByte;
    while(compressed.Read(tmpByte)) {
        src.push_back(tmpByte);
    }
    if(src.empty()) {
        return;
    }
    BitReader reader(src);
    int nElems = reader.readByte();
    int sizeBit = reader.readByte();
    Node * tree = readBitTree(nElems, reader);
    Node * node = tree;
    vector<byte> res;
    map<byte, string> dict = createDict(tree);
    if (nElems != 1) {
        for(size_t i = 0; i < sizeBit; ++i) {
            int bit = reader.readBit();
            if (bit) {
                node = node->right;
            } else {
                node = node->left;
            }
            if(node->left == nullptr && node->right == nullptr) {
                res.push_back(node->symbol);
                node = tree;
            }
        }
    } else {
        for(size_t i = 0; i < sizeBit; ++i) {
            res.push_back(tree->symbol);
        }
    }
    for(const auto & i : res) {
        original.Write(i);
    }
}


int main() {
    VectorInputStream input("В архиве сохраняйте дерево Хаффмана и код Хаффмана от исходных данных. Дерево Хаффмана требуется хранить эффективно - не более 10 бит на каждый 8-битный символ.\n"
                            "В контест необходимо отправить .cpp файл содержащий функции Encode, Decode, а также включающий файл Huffman.h. Тестирующая программа выводит размер сжатого файла в процентах от исходного.");
    VectorOutputStream output;
    Encode(input, output);
    string comp;
    input.buf = output.buf;
    input.c = 0;

    VectorOutputStream output2;
    Decode(input, output2);
    for(const auto & item: output2.buf) {
        cout << item;
    }
    return 0;
}
