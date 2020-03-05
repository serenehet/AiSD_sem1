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
