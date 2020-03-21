#include <iostream>
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
    MyArr(const MyArr &other) = delete;
    MyArr(MyArr &&other) = delete;
    MyArr& operator=(const MyArr &other) = delete;
    MyArr& operator=(MyArr &&other) = delete;
    void pushBack(T element);
    T& operator[](size_t index);
};

template <typename T, typename Comparator = std::less<T>>
class MyHeap {
public:
    explicit MyHeap();
    ~MyHeap();
    void insert(T element);
    T extractMax();
    T peekMax();
private:
    MyArr<T> arr;
    Comparator cmp = Comparator();
    void buildHeap();
    void siftDown(T i);
    void siftUp(T i);
};

int main() {
    MyArr<char> a;
    for (int i = 0; i < 10; ++i) {
        a.pushBack(char(i + 97));
    }
    for (int i = 0; i < 10; ++i) {
        cout << a[i] << " ";
    }
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

// реализация heap

template<typename T, typename Comparator>
MyHeap<T, Comparator>::MyHeap() {
    arr = MyArr<T>();
    cmp = Comparator();
}

template<typename T, typename Comparator>
void MyHeap<T, Comparator>::siftDown(T i) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
// Ищем большего сына, если такой есть.
    int largest = i;
    if( left < arr.size && cmp(arr[left], arr[i]))
        largest = left;
    if( right < arr.size && cmp(arr[right], arr[largest]))
        largest = right;
// Если больший сын есть, то проталкиваем корень в него.
    if( largest != i ) {
        std::swap( arr[i], arr[largest] );
        siftDown(largest);
    }
}
