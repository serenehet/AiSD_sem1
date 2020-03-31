// Алексеев Сергей АПО-13
// Модуль 1 задание 5(4)

// Условие:
// Во всех задачах данного раздела необходимо реализовать и использовать сортировку слиянием.
// Решение должно поддерживать передачу функции сравнения снаружи.
// Общее время работы алгоритма O(n log n).
// На числовой прямой окрасили N отрезков. Известны координаты левого и правого
// концов каждого отрезка (Li и Ri). Найти сумму длин частей числовой прямой,
// окрашенных ровно в один слой.

#include <iostream>
using namespace std;

struct Instruction {
    int point;
    int action;
    explicit Instruction(int x = 0, int act = 0);
};

template <typename T, typename Comparator>
void merge(T * a, size_t aLen, T * b, size_t bLen, T * c, Comparator cmp);

template <typename T, typename Comparator = std::less<T>>
void mergeSort(T * a, size_t aLen, Comparator cmp = Comparator());

size_t getSolve(Instruction * arr, size_t size);

int main() {
    size_t n = 0;
    cin >> n;
    n = n * 2; // обрабатываем каждую точку
    auto * arr = new Instruction[n];
    int act = 1;
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        arr[i] = Instruction(x, act);
        act = -act;
    }
    cout << getSolve(arr, n);
    delete[] arr;
    return 0;
}

Instruction::Instruction(int x, int act) {
    point = x;
    action = act;
}

template <typename T, typename Comparator>
void merge(T * a, size_t aLen, T * b, size_t bLen, T * c, Comparator cmp) {
    size_t aC = 0, bC = 0, counter = 0;
    while((aC < aLen) && (bC < bLen)) {
        if(cmp(a[aC], b[bC])) {
            c[counter++] = a[aC++];
        } else {
            c[counter++] = b[bC++];
        }
    }
    for(size_t i = bC; i < bLen; ++i) {
        c[counter++] = b[bC++];
    }
    for(size_t i = aC; i < aLen; ++i) {
        c[counter++] = a[aC++];
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
    std::copy(c, c + aLen, a);
    delete[] c;
}

size_t getSolve(Instruction * arr, size_t size) {
    mergeSort(arr, size, [](Instruction a, Instruction b) { return a.point < b.point; });
    size_t result = 0;
    int layer = 0, start = arr[0].point, end = arr[size - 1].point;
    int iCurrent = 0;
    for(size_t i = start; i < end; ++i) {
        while (arr[iCurrent].point <= i) {
            layer += arr[iCurrent++].action;
        }
        if (layer == 1) {
            result += 1;
        }
    }
    return result;
}
