#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <stack>

//#include "Huffman.h"

typedef unsigned char byte;
#define interface struct

interface IInputStream {
    // Возвращает false, если поток закончился
    virtual bool Read(byte& value) = 0;
};

interface IOutputStream {
    virtual void Write(byte value) = 0;
};

class MyInputStream : public IInputStream {
public:
    std::vector<byte> buffer;
    int counter;
    MyInputStream(std::string str) : counter(0) {
        for(const auto & item : str) {
            buffer.push_back(item);
        }
    }
    bool Read(byte& value) {
        if (counter >= buffer.size()) { return false; }
        value = buffer[counter++];
        return true;
    }

};

class MyOutputStream : public IOutputStream {
public:
    MyOutputStream() = default;
    void Write(byte value) {}
};


struct Node {
    int n;
    Node * left, * right;
    std::string symbol;
    Node(byte sym, int count) :  n(count), left(nullptr), right(nullptr) {
        symbol += sym;
    }
    Node(Node * t1, Node * t2) : left(t1), right(t2), n(t1->n + t2->n) {}
};


void recurseSearch(std::map<std::string, std::string> & dict, Node *& head, std::string path) {
    Node * node = head;
    if (node->left == nullptr && node->right == nullptr) {
        dict[node->symbol] = path;
        return;
    }
    if (node->right != nullptr) {
        std::string s = path + "1";
        recurseSearch(dict, node->right, s);
    }
    if (node->left != nullptr) {
        std::string s = path + "0";
        recurseSearch(dict, node->left, s);
    }
}

void recurceTreeToBit(Node * head, std::vector<byte> v) {
    
}

void Encode(MyInputStream & original, MyOutputStream & compressed) {
    byte value;
    std::map<char, int> m;
    std::vector<byte> src;
    while (original.Read(value)) {
        src.push_back(value);
        m[value] += 1;
    }
    std::vector<Node *> v;
    for (const auto & item : m) {
        //std::cout << item.first << " " << item.second << std::endl;
        Node * tmp = new Node(item.first, item.second);
        v.push_back(tmp);
    }
    std::make_heap(v.begin(), v.end(), [](Node * t1, Node *t2) {
        return t1->n > t2->n;
    });
//    std::cout << "heap\n";
//    for (const auto & item : v) {
//        std::cout << item->symbol << " " << item->n << "\n";
//    }
    while(v.size() != 1) {
        Node * t1 = v.front();
        std::pop_heap (v.begin(),v.end()); v.pop_back();
        Node * t2 = v.front();
        std::pop_heap(v.begin(),v.end()); v.pop_back();
        Node * tmp = new Node(t1, t2);
        v.push_back(tmp); std::push_heap(v.begin(),v.end());
    }

    std::map<std::string, std::string> dict;
    std::stack<Node *> st;
    std::stack<Node *> path;

    Node * tmp = v.front();
    //std::cout << tmp->left->right->symbol << " suaksmc\n";

    recurseSearch(dict, tmp, "");
    int dictCount = 0;
    for (const auto & item : dict) {
        std::cout << item.first << " " << item.second << std::endl;
        ++dictCount;
    }

}

void Decode(IInputStream& compressed, IOutputStream& original) {

}


int main() {
    std::string str = "abracadabra";
    MyInputStream is(str);
    MyOutputStream os;
    Encode(is, os);
    //Encode(is, MyOutputStream());
    return 0;
}
