//
// Created by Sergei Alexeev on 14.05.2020.
//

#ifndef MODULE2_ZAD5_TEST_H
#define MODULE2_ZAD5_TEST_H

#include<vector>
#include<queue>
#include<string>
#include<algorithm>
#include "Huffman.h"

using namespace std;


struct Node {
    int freq;
    bool is_char = false;
    byte symbol;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    Node(int v, byte b) {
        freq = v;
        symbol = b;
        is_char = true;
    }
    Node(int v) {
        freq = v;
    }
};
class Cmp {
public:
    bool operator() (Node* n1, Node* n2)
    {
        return n1->freq > n2->freq;
    }
};
using myPrQue = std::priority_queue<Node* , std::vector<Node*>, Cmp>;
void dfs(std::vector<std::string> &code, Node* node, std::string str) {
    if (node->is_char) {
        code[node->symbol] = str;
        return;
    }
    dfs(code, node->left, str + "0");
    dfs(code, node->right, str + "1");
}
void WriteCode(std::vector<std::string> &code, byte len,
               IOutputStream& output) {
    //std::ofstream fout("output.txt");
    for (int i = 0; i < 256; i++)
    {
        if (code[i] == "") continue;
        byte symb = i;
        std::vector<byte> Code(len, 0);
        //fout << symb;
        output.Write(symb);
        long long CODE = 0;
        for (auto j : code[i]) {
            CODE = CODE << 1;
            if (j == '1')
                CODE++;
        }
        CODE = CODE << (len * 8 - code[i].size());
        for (int j = len - 1; j >= 0; j--)
        {
            Code[j] = CODE % 256;
            CODE /= 256;
        }
        for (auto j : Code)
            output.Write(j);
        //fout << j;
    }
}
void WriteCompressed(std::vector<std::string> &code, std::vector<byte> &original,
                     IOutputStream& output) {
    //std::ofstream fout("output.txt");
    byte Byte = 0;
    int count = 0;
    for (auto symb : original) {
        for (auto j : code[symb]) {
            Byte = Byte << 1;
            if(j == '1')
                Byte++;
            ++count;
            if (count == 8) {
                //fout << Byte;
                output.Write(Byte);
                Byte = 0;
                count = 0;
            }
        }
    }
    if (count) {
        Byte = Byte << (8 - count);
        //fout << Byte;
        output.Write(Byte);
    }
}
void recursiveDestroi(Node* p) {
    if (p->left != nullptr)
        recursiveDestroi(p->left);
    if (p->right != nullptr)
        recursiveDestroi(p->right);
    delete p;
}
void setHaffTree(myPrQue &q) {
    while (q.size() > 1) {
        Node* n1 = q.top();
        q.pop();
        Node* n2 = q.top();
        q.pop();
        Node* par12 = new Node(n1->freq + n2->freq);
        n1->parent = par12;
        n2->parent = par12;
        par12->left = n1;
        par12->right = n2;
        q.push(par12);
    }
}
void Encode(IInputStream& input, IOutputStream& output)
{
    //std::ifstream fin("input.txt");
    //std::ofstream fout("output.txt");
    //copyStream(original, compressed);
    std::vector<byte> original;
    std::vector<int> counter(256, 0);
    byte b = 0;
    while (input.Read(b)) {
        original.push_back(b);
        counter[b]++;
    }
    myPrQue q;
    byte tablesize = 0;
    for (int i = 0; i < 256; i++)
    {
        if (counter[i] == 0) continue;
        ++tablesize;
        Node* node = new Node(counter[i], i);
        q.push(node);
    }
    setHaffTree(q);
    Node* root = q.top();
    std::vector<std::string> code(286, "");
    dfs(code, root, "");
    byte lengthofcode = 0, extrabits = 0;
    for (auto i : code) {
        lengthofcode = std::max((int)lengthofcode, (int)i.size());
    }
    lengthofcode = (lengthofcode + 7) / 8;
    for (int i = 0; i < 256; i++)
    {
        if (counter[i] == 0) continue;
        extrabits = (extrabits + counter[i] * code[i].size()) % 8;
    }
    extrabits = (8 - extrabits) % 8;
    //fout << extrabits << tablesize << lengthofcode;
    output.Write(extrabits);
    output.Write(tablesize);
    output.Write(lengthofcode);
    WriteCode(code, lengthofcode, output);
    WriteCompressed(code, original, output);
    recursiveDestroi(root);
}

void Decode(IInputStream& input, IOutputStream& output)
{
    //std::ifstream ffin("output.txt");
    //std::ofstream ffout("lookslikeoriginal.txt");
    byte extrabits = 0, tablesize = 0, lengthofcode = 0;
    //ffin >> extrabits >> tablesize >> lengthofcode;
    input.Read(extrabits);
    input.Read(tablesize);
    input.Read(lengthofcode);
    Node* root = new Node(0);
    for (int i = 0; i < tablesize; i++)
    {
        byte symb = 0;
        //ffin >> symb;
        input.Read(symb);
        //std::vector<byte> code(lengthofcode);
        int code = 0;
        for (int j = 0; j < lengthofcode; j++)
        {
            //fin >> code[j];
            byte b = 0;
            //ffin >> b;
            input.Read(b);
            code = (code << 8) + (int)b;
        }
        int n = 1 << (8 * lengthofcode - 1);
        Node* curnode = root;
        for (int j = 0; j < 8 * lengthofcode; j++)
        {
            if (code&n) {//1
                if (curnode->right == nullptr) {
                    Node* newnode = new Node(0);
                    curnode->right = newnode;
                }
                curnode = curnode->right;
            }
            else {//0
                if (curnode->left == nullptr) {
                    Node* newnode = new Node(0);
                    curnode->left = newnode;
                }
                curnode = curnode->left;
            }
            n = n >> 1;
        }
        curnode->is_char = true;
        curnode->symbol = symb;
    }
    byte Byte = 0;
    int n = 1 << 7;
    std::vector<bool> data;
    while (input.Read(Byte)) {
        for (int i = 0; i < 8; i++)
        {
            data.push_back(Byte&n);
            n = n >> 1;
        }
        n = 1 << 7;
    }
    Node* curnode = root;
    int length = data.size() - (int)extrabits;
    for (int i = 0; i < length; i++)
    {
        if (data[i]) {
            curnode = curnode->right;
        }
        else {
            curnode = curnode->left;
        }
        if (curnode->right == nullptr) {
            while (!curnode->is_char)
                curnode = curnode->left;
            //ffout << curnode->symbol;
            output.Write(curnode->symbol);
            curnode = root;
        }
    }
    recursiveDestroi(root);
}

#endif //MODULE2_ZAD5_TEST_H
