// Алексеев Сергей АПО-13
// Модуль 1 задание 4(2)

// Условие:
// Для сложения чисел используется старый компьютер.
// Время, затрачиваемое на нахождение суммы двух чисел равно их сумме.
// Таким образом для нахождения суммы чисел 1,2,3 может потребоваться
// разное время, в зависимости от порядка вычислений.
// ((1+2)+3) -> 1+2 + 3+3 = 9 ((1+3)+2) -> 1+3 + 4+2 = 10 ((2+3)+1) -> 2+3 + 5+1 = 11
// Требуется написать программу, которая определяет минимальное время,
// достаточное для вычисления суммы заданного набора чисел.
// Требуемое время работы O(n*log(n)).

#include <iostream>
#include <cassert>
#include <cstdio>
using namespace std;

template<class T>
class MyArr {
private:
    size_t sizeArr;
    T * arr;
public:
    size_t size;
    explicit MyArr();
    ~MyArr();
    bool isEmpty();
    void pushBack(T element);
    T& operator[](size_t index);
    void deleteLast();
    T last();
};

template <typename T, typename Comparator = std::less<T>>
class MyHeap {
public:
    explicit MyHeap();
    explicit MyHeap(MyArr<T> &newArr);
    ~MyHeap();
    bool isEmpty();
    void add(T element);
    T extractElem();
    T peekElem();
private:
    MyArr<T> arr;
    Comparator cmp = Comparator();
    void buildHeap();
    void siftDown(int i);
    void siftUp(int i);
};

int getTimeCalculate(MyArr<int> & arr);

int main() {
    // получаем данные
    size_t n;
    cin >> n;
    MyArr<int> arr;
    for(int i = 0; i < n; ++i) {
        int temp;
        cin >> temp;
        arr.pushBack(temp);
    }
    //печатаем решение
    cout << getTimeCalculate(arr);
    return 0;
}

// решение задачи
int getTimeCalculate(MyArr<int> & arr) {
    MyHeap<int, std::greater<>> heap(arr);
    int c = 0;
    while (!heap.isEmpty()) {
        int temp = heap.extractElem();
        if (!heap.isEmpty()) {
            temp += heap.extractElem();
            heap.add(temp);
            c += temp;
        }
    }
    return c;
}

// реализация вектора

template<class T>
MyArr<T>::MyArr() {
    sizeArr = 0;
    size = 0;
    arr = nullptr;
}

template<class T>
MyArr<T>::~MyArr() {
    size = 0;
    sizeArr = 0;
    delete[] arr;
}

template<class T>
void MyArr<T>::pushBack(T element) {
    if (isEmpty()) {
        size = 1;
        sizeArr = 1;
        arr = new T[sizeArr];
        arr[0] = element;
        return;
    }
    if (size == sizeArr) {
        T * tempArr = new T[sizeArr * 2];
        std::copy(arr, arr + sizeArr, tempArr);
        delete[] arr;
        arr = tempArr;
        sizeArr *= 2;
    }
    arr[size++] = element;
}

template<class T>
T & MyArr<T>::operator[](size_t index) {
    return arr[index];
}

template<class T>
bool MyArr<T>::isEmpty() {
    return (size == 0);
}

template<class T>
void MyArr<T>::deleteLast() {
    if (isEmpty()) { return; }
    arr[size--] = 0;
}

template<class T>
T MyArr<T>::last() {
    return arr[size - 1];
}

// реализация heap

template<typename T, typename Comparator>
MyHeap<T, Comparator>::MyHeap() {
    arr = MyArr<T>();
    cmp = Comparator();
}

template<typename T, typename Comparator>
void MyHeap<T, Comparator>::siftDown(int i) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    // Ищем большего сына, если такой есть.
    int largest = i;
    if( left < arr.size && !cmp(arr[left], arr[i]))
        largest = left;
    if( right < arr.size && !cmp(arr[right], arr[largest]))
        largest = right;
    // Если больший сын есть, то проталкиваем корень в него.
    if( largest != i ) {
        std::swap( arr[i], arr[largest] );
        siftDown(largest);
    }
}

template<typename T, typename Comparator>
void MyHeap<T, Comparator>::buildHeap() {
    for(int i = arr.size / 2 - 1; i >= 0; --i ) {
        siftDown(i);
    }
}

template<typename T, typename Comparator>
void MyHeap<T, Comparator>::siftUp(int index) {
    while( index > 0 ) {
        int parent = ( index - 1 ) / 2;
        if(cmp(arr[index], arr[parent]))
            return;
        std::swap( arr[index], arr[parent] );
        index = parent;
    }
}

template<typename T, typename Comparator>
void MyHeap<T, Comparator>::add(T element) {
    arr.pushBack(element);
    siftUp(arr.size - 1);
}

template<typename T, typename Comparator>
T MyHeap<T, Comparator>::extractElem() {
    assert( !arr.isEmpty() );
    // Запоминаем значение корня.
    int result = arr[0];
    // Переносим последний элемент в корень.
    arr[0] = arr.last();
    arr.deleteLast();
    // Вызываем SiftDown для корня.
    if( !arr.isEmpty() ) {
        siftDown( 0 );
    }
    return result;
}

template<typename T, typename Comparator>
T MyHeap<T, Comparator>::peekElem() {
    return arr[0];
}

template<typename T, typename Comparator>
bool MyHeap<T, Comparator>::isEmpty() {
    return arr.isEmpty();
}

template<typename T, typename Comparator>
MyHeap<T, Comparator>::~MyHeap() {
    while (!arr.isEmpty()) {
        arr.deleteLast();
    }
}

template<typename T, typename Comparator>
MyHeap<T, Comparator>::MyHeap(MyArr<T> &newArr) {
    cmp = Comparator();
    arr = newArr;
    buildHeap();
}
