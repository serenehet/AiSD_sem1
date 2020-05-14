//
// Created by Sergei Alexeev on 14.05.2020.
//


#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <stack>
#include <queue>

#include "Huffman.h"

using namespace std;

class BitWriter {
public:
    BitWriter() : bitCounter(0) {}
    void WriteBit(byte bit) {
        if (bitCounter % 8 == 0)
            buffer.push_back(0);
        if (bit)
            buffer[bitCounter/8] |= 1 << (7 - bitCounter % 8);
        bitCounter++;
    }
    void WriteByte(byte byte) {
        if (bitCounter % 8 == 0) {
            buffer.push_back(byte);
        }
        else {
            int offset = bitCounter % 8;
            buffer[bitCounter/8] |= byte >> offset;
            buffer.push_back(byte << (8 - offset));
        }
        bitCounter += 8;
    }
    const vector<byte> & GetBuffer() const {
        return buffer;
    }
    size_t GetBitCounter() const {
        return bitCounter;
    }
private:
    vector<byte> buffer;
    size_t bitCounter;
};

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
    int freq;
    byte symbol;
    Node* left = nullptr;
    Node* right = nullptr;
    Node(int n = 0, byte b = 0) : freq(n), symbol(b){}
};

class Cmp { // для очереди с приоритетом, которая хранить Node *
public:
    bool operator() (Node* n1, Node* n2) {
        return n1->freq > n2->freq;
    }
};

void setHaffTree(priority_queue<Node * , vector<Node*>, Cmp> & q) {
    while (q.size() > 1) {
        Node* n1 = q.top(); q.pop();
        Node* n2 = q.top(); q.pop();
        Node* combo = new Node(n1->freq + n2->freq);
        combo->left = n1;
        combo->right = n2;
        q.push(combo);
    }
}

void setDict(map<byte, string> & dict, Node *& node, string path) {
    if(node->left == nullptr && node->right == nullptr) {
        dict[node->symbol] = path;
        return;
    }
    setDict(dict, node->left, path + "0");
    setDict(dict, node->right, path + "1");
}

void recursiveDestroi(Node * node) {
    if (node == nullptr) {
        return;
    }
    recursiveDestroi(node->left);
    recursiveDestroi(node->right);
    delete node;
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

void Encode(IInputStream& original, IOutputStream& compressed) {
    vector<byte> src;
    map<byte, int> counter;
    byte b = 0;
    while (original.Read(b)) {
        src.push_back(b);
        counter[b]++;
    }
    priority_queue<Node * , vector<Node*>, Cmp> q;
    for(const auto & item: counter) {
        Node* node = new Node(item.second, item.first);
        q.push(node);
    }
    byte nInTree = q.size();
    setHaffTree(q);
    Node * tree = q.top();
    map<byte, string> dict;
    setDict(dict, tree, "");
    BitWriter writer;
    int bits = 0;
    for(const auto & item : dict) {
        bits += item.second.size() * counter[item.first];
    }

    writer.WriteByte(nInTree);
    writer.WriteByte(bits >> 16);
    writer.WriteByte(bits >> 8);
    writer.WriteByte(bits);

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
    for(const auto & i : writer.GetBuffer()) {
        compressed.Write(i);
    }
    recursiveDestroi(tree);
}

Node * readBitTree(int n, BitReader & reader) {
    stack<Node *> s;
    size_t counter = 0;
    while(counter < n || s.size() != 1) {
        bool bit = reader.readBit();
        if(bit) {
            byte symbol = reader.readByte();
            Node * node = new Node(0, symbol);
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

void Decode(IInputStream& compressed, IOutputStream& original) {
    vector<byte> src;
    byte b;
    while(compressed.Read(b)) {
        src.push_back(b);
    }
    BitReader reader(src);
    int nInTree = (int)reader.readByte();
    if(nInTree == 0) {nInTree = 256;}
    int bits = (reader.readByte() << 16) + (reader.readByte() << 8) + reader.readByte();
    Node * tree = readBitTree(nInTree, reader);
    Node * node = tree;
    vector<byte> res;
    for(size_t i = 0; i < bits; ++i) {
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
    recursiveDestroi(tree);
    for(const auto & i : res) {
        original.Write(i);
    }
}