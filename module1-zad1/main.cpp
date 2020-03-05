#include <iostream>
using namespace std;
int main() {
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
    cout << counter;
    delete[] a;
    delete[] b;
    return 0;
}
