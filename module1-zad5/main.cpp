#include <iostream>
using namespace std;

template <typename T, typename Comparator>
void merge(T * a, size_t aLen, T * b, size_t bLen, T * c, Comparator cmp);
template <typename T, typename Comparator = std::less<T>>
void mergeSort(T * a, size_t aLen, Comparator cmp = Comparator());

struct Instruction {
    int point;
    int action;
    Instruction(int x = 0, int act = 0);
};

void printSolve();

int main() {
    int n = 0;
    cin >> n;
    Instruction * arr = new Instruction[n * 2];
    int act = 1;
    for (int i = 0; i < n * 2; ++i) {
        int x;
        cin >> x;
        arr[i] = Instruction(x, act);
        act = -act;
    }
    for(int i = 0; i < n * 2; i++) {
        cout << arr[i].point << " - point " << arr[i].action << " - act\n";
    }
    mergeSort(arr, n * 2, [](Instruction a, Instruction b) { return a.point < b.point; });
    cout << "---\n";
    for(int i = 0; i < n * 2; i++) {
        cout << arr[i].point << " - point " << arr[i].action << " - act\n";
    }
    return 0;
}

template <typename T, typename Comparator>
void merge(T * a, size_t aLen, T * b, size_t bLen, T * c, Comparator cmp) {
    size_t size = aLen + bLen;
    size_t counter = 0;
    size_t aC = 0;
    size_t bC = 0;
    while ((counter != size) && (bC != bLen) && (aC != aLen))  {
        if (cmp(a[aC], b[bC])) {
            c[counter++] = a[aC++];
        } else {
            c[counter++] = b[bC++];
        }
    }
    if (aC == aLen) {
        for (size_t i = bC; i < bLen; ++i) {
            c[counter++] = b[bC];
        }
    } else {
        for (size_t i = aC; i < aLen; ++i) {
            c[counter++] = a[aC];
        }
    }
}

template <typename T, typename Comparator>
void mergeSort(T * a, size_t aLen, Comparator cmp) {
    if( aLen <= 1 ) { return; }
    size_t firstLen = aLen / 2;
    size_t secondLen = aLen - firstLen;
    mergeSort(a, firstLen, cmp);
    mergeSort(a + firstLen, secondLen, cmp);
    T * c = new T[aLen];
    merge(a, firstLen, a + firstLen, secondLen, c, cmp);
    std::copy(c, c + aLen - 1, a);
    delete[] c;
}

Instruction::Instruction(int x, int act) {
    point = x;
    action = act;
}