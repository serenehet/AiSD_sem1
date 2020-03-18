// Алексеев Сергей АПО-13
// Модуль 1 задание 5(4)

// Условие:
//Во всех задачах данного раздела необходимо реализовать и использовать сортировку слиянием.
//Решение должно поддерживать передачу функции сравнения снаружи.
//Общее время работы алгоритма O(n log n).
//На числовой прямой окрасили N отрезков. Известны координаты левого и правого концов каждого отрезка (Li и Ri). Найти сумму длин частей числовой прямой, окрашенных ровно в один слой.

#include <iostream>
using namespace std;

template <typename T, typename Comparator>
void merge(T * a, size_t aLen, T * b, size_t bLen, T * c, Comparator cmp);
template <typename T, typename Comparator = std::less<T>>
void mergeSort(T * a, size_t aLen, Comparator cmp = Comparator());

struct Instruction {
    int point;
    int action;
    explicit Instruction(int x = 0, int act = 0);
};

int main() {
    int n = 0;
    cin >> n;
    size_t size = n * 2;
    Instruction * arr = new Instruction[n * 2];
    int act = 1;
    for (int i = 0; i < n * 2; ++i) {
        int x;
        cin >> x;
        arr[i] = Instruction(x, act);
        act = -act;
    }
    mergeSort(arr, n * 2, [](Instruction a, Instruction b) { return a.point < b.point; });
    int kek = 0;
    int counter = 0;
    int start = arr[0].point;
    int end = arr[size - 1].point;
    int j = 0;
    for(size_t i = start; i <= end; ++i) {
        while (arr[j].point <= i) {
            counter += arr[j++].action;
        }
        if (counter == 1) {
            kek += 1;
        }
    }
    cout << kek;
    delete[] arr;
    return 0;
}

template <typename T, typename Comparator>
void merge(T * a, size_t aLen, T * b, size_t bLen, T * c, Comparator cmp) {

    int i = 0, j = 0, k = 0;

    while(i < aLen && j < bLen) {
        if(cmp(a[i],b[j])) c[k++] = a[i++];
        else c[k++] = b[j++];
    }
    while(j < bLen) c[k++] = b[j++];
    while(i < aLen) c[k++] = a[i++];

//    size_t size = aLen + bLen;
//    size_t counter = 0;
//    size_t aC = 0;
//    size_t bC = 0;
//    while ((counter != size) && (bC != bLen) && (aC != aLen))  {
//        if (cmp(a[aC], b[bC])) {
//            c[counter++] = a[aC++];
//        } else {
//            c[counter++] = b[bC++];
//        }
//    }
//    if (aC == aLen) {
//        for (size_t i = bC; i < bLen; ++i) {
//            c[counter++] = b[bC];
//        }
//    } else {
//        for (size_t i = aC; i < aLen; ++i) {
//            c[counter++] = a[aC];
//        }
//    }
}

template <typename T, typename Comparator>
void mergeSort(T * a, size_t aLen, Comparator cmp) {
    if( aLen <= 1 ) {
        return;
    }
    int firstLen = aLen / 2;
    int secondLen = aLen - firstLen;
    mergeSort(a, firstLen, cmp);
    mergeSort( a + firstLen, secondLen,cmp);
    T *c = new T[aLen];
    merge( a, firstLen, a + firstLen, secondLen, c ,cmp);
    std::copy(c, c + aLen, a);
//    for(int i = 0; i < aLen; ++i){
//        a[i] = c[i];
//    }
    delete[] c;
}

Instruction::Instruction(int x, int act) {
    point = x;
    action = act;
}