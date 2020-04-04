// Алексеев Сергей АПО-13
// Модуль 2 задание 1(1)

// Условие:
// Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
// Хранимые строки непустые и состоят из строчных латинских букв.
// Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
// Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
// когда коэффициент заполнения таблицы достигает 3/4.
// Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества
// и проверки принадлежности данной строки множеству.

// 1_1. Для разрешения коллизий используйте квадратичное пробирование.
// i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.


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
        for (const char & i : str) {
            hash = hash * prime + i;
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
        size = 0;
        arr.clear();
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
            if (item != deleted) {
                size_t index = hasher(item) % newArr.size();
                size_t i = 0;
                while(newArr[index] != nil) {
                    index = (size_t)(index + 0.5 * i * (i + 1)) % newArr.size();
                    ++i;
                }
                newArr[index] = item;
            }
        }
        arr = newArr;
    }
};

int main(int argc, const char * argv[]) {
    HashTable<StringHasher> table;
    char cmd;
    string str;
    while (cin >> cmd >> str) {
        switch (cmd) {
            case '?': {
                cout << (table.has(str) ? "OK" : "FAIL") << endl;
                break;
            }
            case '+': {
                cout << (table.add(str) ? "OK" : "FAIL") << endl;
                break;
            }
            case '-': {
                cout << (table.remove(str) ? "OK" : "FAIL") << endl;
                break;
            }
        }
    }
    return 0;
}
