#include <iostream>
#include <vector>
#include <string>
using namespace std;

const size_t DEFAULT_SIZE = 8;
const double MAX_ALPHA = 0.75;
const string nil = "NIL";
const string deleted = "DEL";

class StringHasher {
public:
    StringHasher(size_t prime = 71): prime(prime) {}
    size_t operator()(const std::string &str)
    {
        size_t hash = 0;
        for (size_t i = 0; i < str.size(); i++) {
            hash = hash * prime + str[i];
        }

        return hash;
    }
private:
    size_t prime;
};

template <typename Hasher>
class HashTable {
public:
    HashTable(): size(0), arr(DEFAULT_SIZE, nil) {}
    ~HashTable() {

    }

    void print() {
        for(size_t i = 0; i < arr.size(); ++i) {
            cout << i << ": " << arr[i] << endl;
        }
    }

    bool add(string element) {
        if (((double)size / arr.size()) >= MAX_ALPHA) {
            reSize(arr.size() * 2);
        }

        size_t i = 0;
        size_t hash = hasher(element) % arr.size();
        size_t index = hash;
        int indexDelete = -1;
        while(i <= arr.size() && arr[index] != nil) {
            if (arr[index] == element) {
                return false;
            }

            if (arr[index] == deleted && indexDelete == -1) {
                indexDelete = index;
            }

            index = (size_t)(index + 0.5 * i * (i + 1)) % arr.size();
            ++i;
        }
        if (indexDelete == -1) {
            arr[index] = element;
        } else {
            arr[indexDelete] = element;
        }
        size++;
        return true;
    }

    bool has(string element) {
        size_t i = 0;
        size_t index = hasher(element) % arr.size();
        while(i < arr.size() && arr[index] != nil) {
            if (arr[index] == element) {
                return true;
            }
            index = (size_t)(index + 0.5 * i * (i + 1)) % arr.size();
            ++i;
        }
        return false;
    }

    bool remove(string element) {
        size_t i = 0;
        size_t index = hasher(element) % arr.size();
        while(i < arr.size() && arr[index] != nil) {
            if (arr[index] == element) {
                --size;
                arr[index] = deleted;
                return true;
            }
            index = (size_t)(index + 0.5 * i * (i + 1)) % arr.size();
            ++i;
        }
        return false;
    }

private:
    vector<string> arr;
    size_t size;
    Hasher hasher;

    void reSize(size_t newSize) {
        vector<string> newArr(newSize, nil);
        for(const auto & item : arr) {
            size_t index = hasher(item) % newArr.size();
            size_t i = 0;
            while(newArr[index] != nil) {
                index = (size_t)(index + 0.5 * i * (i + 1)) % newArr.size();
            }
            newArr[index] = item;
        }
        arr = newArr;
    }
};

int main(int argc, const char * argv[]) {
    HashTable<StringHasher> table;
    char comand;
    string str;
    while (cin >> comand >> str) {
        switch (comand) {
            case '?': {
                cout << (table.has(str) ? "OK" : "FAIL") << endl;
//                table.print();
                break;
            }
            case '+': {
                cout << (table.add(str) ? "OK" : "FAIL") << endl;
//                table.print();
                break;
            }
            case '-': {
                cout << (table.remove(str) ? "OK" : "FAIL") << endl;
//                table.print();
                break;
            }
        }
    }

    return 0;
}
