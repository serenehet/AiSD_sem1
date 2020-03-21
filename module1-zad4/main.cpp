#include <iostream>
#include <assert.h>
using namespace std;

template<class T>
class MyArr {
private:
    size_t sizeArr;
    T * arr;
public:
    size_t size;
    explicit MyArr();
    bool isEmpty();
    MyArr(T * newArray, size_t n);
    ~MyArr();
    void pushBack(T element);
    T& operator[](size_t index);
    void deleteLast();
    T last();
};

template <typename T, typename Comparator = std::less<T>>
class MyHeap {
public:
    explicit MyHeap();
    ~MyHeap();
    bool isEmpty();
    void insert(T element);
    T extractMax();
    T peekMax();
private:
    MyArr<T> arr;
    Comparator cmp = Comparator();
    void buildHeap();
    void siftDown(int i);
    void siftUp(int i);
};

bool kek(int a, int b) {
    return a <= b;
}
int main() {
    int n = 0;
    cin >> n;
    auto cmp = [](int a, int b) { return a < b;};
    MyHeap<int, std::greater<int>> heap;
    for(int i = 0; i < n; ++i) {
        int temp;
        cin >> temp;
        heap.insert(temp);
    }
    int c = 0;
    while (!heap.isEmpty()) {
        int temp = heap.extractMax();
        if (!heap.isEmpty()) {
            temp += heap.extractMax();
            heap.insert(temp);
            c += temp;
        }
    }
    cout << c << " ";
    return 0;
}

// реализация вектора

template<class T>
MyArr<T>::MyArr() {
    sizeArr = 0;
    size = 0;
    arr = nullptr;
}


template<class T>
MyArr<T>::MyArr(T * newArray, size_t n) {
    sizeArr = 1;
    while (sizeArr < n) {
        sizeArr *= 2;
    }
    arr = new int[sizeArr];
    size = n;
    memcpy(arr, newArray, n * sizeof(int));
    for (size_t i = 0; i < size; ++i) {
        arr[i] = newArray[i];
    }
}

template<class T>
MyArr<T>::~MyArr() {
    delete[] arr;
    size = 0;
    sizeArr = 0;
}

template<class T>
void MyArr<T>::pushBack(T element) {
    if (size == sizeArr) {
        T * tempArr = new T[sizeArr * 2];
        memcpy(tempArr, arr, sizeArr * sizeof(int));
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
void MyHeap<T, Comparator>::insert(T element) {
    arr.pushBack(element);
    siftUp(arr.size - 1);
}

template<typename T, typename Comparator>
T MyHeap<T, Comparator>::extractMax() {
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
T MyHeap<T, Comparator>::peekMax() {
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
