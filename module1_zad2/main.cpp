//2_3. Даны два массива неповторяющихся целых чисел, упорядоченные по возрастанию.
//A[0..n-1] и B[0..m-1]. n >> m. Найдите их пересечение.
//Требования: Время работы: O(m * log k), где k - позиция элемента B[m-1] в массиве A..
//В процессе поиска очередного элемента B[i] в массиве A пользуйтесь результатом поиска элемента B[i-1].
//Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска размером порядка k,
//а потом уже в нем делать бинарный поиск.

#include <iostream>
using namespace std;

int binarySearch(const int * arr, int count, int element);
void printIntersection(const int * arr1, const int * arr2, int size1, int size2);

int main() {
    int n, m;
    cin >> n >> m;
    int *a = new int[n];
    int *b = new int[m];
    for (size_t i = 0; i < n; ++i) {
        cin >> a[i];
    }
    for (size_t i = 0; i < m; ++i) {
        cin >> b[i];
    }
    printIntersection(a, b, n, m);

    delete[] a;
    delete[] b;
    return 0;
}

int binarySearch(const int * arr, int count, int element) {
    int first = 0;
    int last = count;
    while (first < last) {
        int mid = (first + last) / 2;
        if (arr[mid] < element)
            first = mid + 1;
        else
            last = mid;
    }
    return (first == count || arr[first] != element) ? -1 : first;
}

void printIntersection(const int * arr1, const int * arr2, const int size1, const int size2) {
    int r = 1;
    for (size_t i = 0; i < size2; ++i) {
        while (arr1[r] < arr2[i] && r < size1) {
            r *= 2;
        }
        int l = r / 2;
        if (r >= size1) {
            r = size1 - 1;
        }
        if(binarySearch(arr1 + l, r - l + 1, arr2[i]) != -1)
            cout << arr2[i] << " ";
    }
}