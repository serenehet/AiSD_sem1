// Алексеев Сергей АПО-13
// Модуль 1 задание 1(3)

// Условие:
//Даны два строго возрастающих массива целых чисел A[0..n) и B[0..m) и число k.
// Найти количество таких пар индексов (i, j), что A[i] + B[j] = k.
// Время работы O(n + m). n, m ≤ 100000.
// Указание. Обходите массив B от конца к началу.

#include <iostream>
using namespace std;

size_t numberOfEqK(const int * const a, size_t n, const int * const b, size_t m, int k);

int main() {
    // собираем данные
    int n = 0, m = 0, k = 0;
    cin >> n;
    int *a = new int[n];
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    cin >> m;
    int *b = new int[m];
    for (int i = 0; i < m; ++i) {
        cin >> b[i];
    }
    cin >> k;
    // печатаем решение
    cout << numberOfEqK(a, n, b, m, k);
    // освободим память
    delete[] a;
    delete[] b;
    return 0;
}

size_t numberOfEqK(const int * const a, size_t n, const int * const b, size_t m, int k) {
    int ia = 0;
    int ib = m - 1;
    int counter = 0;
    while (ia != n && ib != -1) {
        if (a[ia] + b[ib] == k) {
            ++ia;
            --ib;
            counter++;
        } else if (a[ia] + b[ib] > k) {
            --ib;
        } else if (a[ia] + b[ib] < k) {
            ++ia;
        }
    }
    return counter;
}
