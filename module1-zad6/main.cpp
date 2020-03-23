// Алексеев Сергей АПО-13
// Модуль 1 задание 6(1)

// Условие:
// Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n.
// Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло
// на позиции с индексом k (0..n-1) в отсортированном массиве.
//
//Требования: к дополнительной памяти: O(n).
// Среднее время работы: O(n). Должна быть отдельно выделенная функция partition.
// Рекурсия запрещена. Решение должно поддерживать передачу функции сравнения снаружи.
//
// Реализуйте стратегию выбора опорного элемента “медиана трёх”.
// Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.


#include <iostream>
#include <functional>
#include <cmath>
using namespace std;

template <typename T, typename Comparator>
size_t getIndexMedian(T * arr, size_t a, size_t b, size_t c, Comparator cmp = Comparator()) {
    if ((cmp(arr[a], arr[b]) && cmp(arr[b], arr[c])) || (!cmp(arr[a], arr[b]) && !cmp(arr[b], arr[c]))) {
        return b;
    }
    if ((cmp(arr[a], arr[c]) && cmp(arr[c], arr[b])) || (!cmp(arr[a], arr[c]) && !cmp(arr[c], arr[b]))) {
        return c;
    }
    return a;
}

template <typename T, typename Comparator = std::less<T>>
int partition(T *arr, int l, int r, Comparator cmp = Comparator()) {
    swap(arr[r], arr[getIndexMedian(arr, l, (l + r) / 2, r, cmp)]);
    int pivot = arr[r];
    int i = l;
    int j = l;
    int c = l;
    for(size_t t = l; t < r; ++t) {
        if (cmp(pivot, arr[t])) {
            j++;
        } else {
            swap(arr[i++], arr[j++]);
        }
    }
    swap(arr[i], arr[r]);
    return i;
}

template <typename T, typename Comparator = std::less<T>>
T kStatistic(T *arr, int l, int r, int k, Comparator cmp = Comparator()) {
    int pivotPos = partition(arr, l, r, cmp);
    int tempL = l, tempR = r;
    while (pivotPos != k) {
        if (pivotPos < k) {
            tempL = pivotPos + 1;
        } else {
            tempR = pivotPos - 1;
        }
        pivotPos = partition(arr, tempL, tempR, cmp);
    }
    return arr[pivotPos];
}

int main(int argc, const char * argv[]) {
    // получаем данные
    int n, k;
    cin >> n >> k;
    int * arr = new int[n];
    for(size_t i = 0; i < n; ++i) {
        int temp;
        cin >> temp;
        arr[i] = temp;
    }
    //печатаем решение
    cout << kStatistic(arr, 0, n - 1, k, less<>());
    delete[] arr;
    return 0;
}
