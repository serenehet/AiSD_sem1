// Алексеев Сергей АПО-13
// Модуль 1 задание 3(2)

// Условие:
// Реализовать дек с динамическим зацикленным буфером.
// Формат входных данных:
// В первой строке количество команд n. n ≤ 1000000.
// Каждая команда задаётся как 2 целых числа: a b.
// a = 1 - push front
// a = 2 - pop front
// a = 3 - push back
// a = 4 - pop back
// Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
// Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
// Если дана команда pop*, то число b - ожидаемое значение. Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.
// Формат выходных данных.
// Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.

#include <iostream>
#include <cstring>
using namespace std;

class Deck {
private:
    int iTail;
    int iHead;
    int * array;
    size_t size;
    void print() const;
public:
    explicit Deck();
    ~Deck();
    Deck(const Deck &other) = delete;
    Deck(Deck &&other) = delete;
    Deck& operator=(const Deck &other) = delete;
    Deck& operator=(Deck &&other) = delete;
    bool isEmpty();
    void pushFront(int element);
    int popFront();
    void pushBack(int element);
    int popBack();
};

int main() {
    int n;
    cin >> n;
    bool key = true;
    Deck deck;
    for (size_t i = 0; i < n; ++i) {
        int comand, arg;
        cin >> comand >> arg;
        if (comand == 1) {
            deck.pushFront(arg);
        }
        if (comand == 2) {
            key = key & (arg == deck.popFront());
        }
        if (comand == 3) {
            deck.pushBack(arg);
        }
        if (comand == 4) {
            key = key & (arg == deck.popBack());
        }
    }
    if (key) {
        cout << "YES";
    } else {
        cout << "NO";
    }
    return 0;
}

Deck::Deck() {
    size = 0;
    array = nullptr;
    iTail = -1;
    iHead = -1;
}

Deck::~Deck() {
    delete[] array;
    size = 0;
    iTail = -1;
    iHead = -1;
}

bool Deck::isEmpty() {
    return iHead == -1;
}

void Deck::pushFront(int element) {
    if (isEmpty()) {
        if (size == 0) {
            array = new int[1];
            size = 1;
        }
        iHead = 0;
        iTail = 0;
        array[iHead] = element;
        return;
    }
    int tempHead = iHead - 1;
    if (tempHead < 0) {tempHead = (int)size - 1; }
    if (tempHead == iTail) {
        int * newArray = new int[size * 2];
        if (iHead == 0) {
            memcpy(newArray, array, size * sizeof(int));
            iHead = ((int)size * 2) - 1;
            newArray[iHead] = element;
        } else {
            memcpy(newArray, array, (iTail + 1) * sizeof(int));
            memcpy(newArray + size + iTail + 1, array + iHead, (size - iHead) * sizeof(int));
            iHead += size;
            newArray[--iHead] = element;
        }
        size *= 2;
        delete[] array;
        array = newArray;
        return;
    }
    array[tempHead] = element;
    iHead = tempHead;
}

int Deck::popFront() {
    if (isEmpty()) { return -1; }
    if (iHead == iTail) {
        int res = array[iHead];
        array[iHead] = 0;
        iHead = iTail = -1;
        return res;
    }
    int res = array[iHead++];
    if (iHead == size) {
        iHead = 0;
    }
    return res;
}

void Deck::pushBack(int element) {
    if (isEmpty()) {
        if (size == 0) {
            array = new int[1];
            size = 1;
        }
        iHead = 0;
        iTail = 0;
        array[iTail] = element;
        return;
    }

    int tempTail = iTail + 1;
    if (tempTail == size) {tempTail = 0;}
    if (tempTail == iHead) {
        int * newArray = new int[size * 2];
        if (iTail == (int)size - 1) {
            memcpy(newArray, array, size * sizeof(int));
            newArray[++iTail] = element;
        } else {
            memcpy(newArray, array, (iTail + 1) * sizeof(int));
            memcpy(newArray + size + iTail + 1, array + iHead, (size - iHead) * sizeof(int));
            iHead += size;
            newArray[++iTail] = element;
        }
        size *= 2;
        delete[] array;
        array = newArray;

        return;
    }
    array[tempTail] = element;
    iTail = tempTail;
}

int Deck::popBack() {
    if (isEmpty()) { return -1; }
    if (iHead == iTail) {
        int res = array[iTail];
        array[iTail] = 0;
        iTail = iHead = -1;
        return res;
    }
    int res = array[iTail--];
    if (iTail == -1) {
        iTail = (int)size - 1;
    }
    return res;
}

void Deck::print() const {
    cout << "---\n";
    for(size_t i = 0; i < size; ++i) {
        if (iTail == i) {
            cout << "iTail: ";
        }
        if (iHead == i) {
            cout << "iHead: ";
        }
        cout << " " << array[i] << "\n";
    }
}
