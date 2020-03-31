// Алексеев Сергей АПО-13
// Модуль 1 задание 7(2)

// Условие:
// Дан массив неотрицательных целых 64-битных чисел.
// Количество чисел не больше 1000000. Отсортировать массив методом
// поразрядной сортировки LSD по байтам.

#include <iostream>
#include <cassert>
using namespace std;

int byteN(const long long & numeric, const size_t & numberByte);

void byteSort(long long *arr, const size_t & size, const size_t & numberByte);

void lsdSort(long long *arr, size_t size);

int main() {
    // получаем данные
    size_t n = 0;
    cin >> n;
    auto *arr = new long long[n];
    for(size_t i = 0; i < n; ++i) {
        cin >> arr[i];
    }
    // решаем задачу
    lsdSort(arr, n);
    // печатаем ответ
    for(size_t i = 0; i < n; ++i) {
        cout << arr[i] << " ";
    }
    //очищаем память
    delete[] arr;
    return 0;
}

int byteN(const long long & numeric, const size_t & numberByte) {
    assert(numberByte < 8);
    int res = numeric >> (8 * numberByte) & 255;
    return res;
}

void byteSort(long long *arr, const size_t & size, const size_t & numberByte) {
    const size_t nMax = 256;    // по условию
    auto * counterArr = new size_t[nMax];
    for(size_t i = 0; i < nMax; i++) {
        counterArr[i] = 0;
    }
    for(size_t i = 0; i < size; i++) {
        counterArr[byteN(arr[i], numberByte)]++;
    }
    for(size_t i = 1; i < nMax; i++) {
        counterArr[i] += counterArr[i - 1];
    }

    auto *res = new long long[size];
    for(int i = (int)size - 1; i >= 0; --i) {
        size_t index = --(counterArr[byteN(arr[i], numberByte)]);
        res[index] = arr[i];
    }
    std::copy(res, res + size, arr);
    delete[] res;

    delete[] counterArr;
}

void lsdSort(long long *arr, size_t size) {
    const size_t nBytes = 8;  // по условию
    for(size_t i = 0; i < nBytes; ++i) {
        byteSort(arr, size, i);
    }
}
