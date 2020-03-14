#include <iostream>
#include <assert.h>
using namespace std;

class SerenArray {
private:
    size_t sizeArr;
    int * arr;
public:
    size_t size;
    SerenArray();
    bool isEmpty();
    SerenArray(int * newArray, size_t n);
    ~SerenArray();
    SerenArray(const SerenArray &other) = delete;
    SerenArray(SerenArray &&other) = delete;
    SerenArray& operator=(const SerenArray &other) = delete;
    SerenArray& operator=(SerenArray &&other) = delete;
    void pushBack(int element);
    int& operator[](const int index);
};

class SerenHeap {
public:
    explicit SerenHeap();
    explicit SerenHeap(const SerenArray & array);
    ~SerenHeap();
    void insert( int element ) {
        arr.pushBack(element);
        siftUp(arr.size - 1);
    }
    // Извлечение максимального элемента.
    int extractMax() {
        assert( !arr.IsEmpty() );
// Запоминаем значение корня.
        int result = arr[0];
// Переносим последний элемент в корень.
        arr[0] = arr.Last();
        arr.DeleteLast();
// Вызываем SiftDown для корня.
        if( !arr.IsEmpty() ) {
            siftDown( 0 );
        }
        return result;
    }
    int peekMax() const;
private:
    SerenArray arr;
    void buildHeap() {
        for( int i = arr.size / 2 - 1; i >= 0; --i ) {
            siftDown(i);
        }
    }
    void siftUp( int index ){
        while( index > 0 ) {
            int parent = ( index - 1 ) / 2;
            if( arr[index] <= arr[parent] )
                return;
            std::swap( arr[index], arr[parent] );
            index = parent;
        }
    }
    // Построение кучи.

    void siftDown(int i) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        //ищем большего сына, если такой есть.
        int largest = i;
        if( left < arr.size && arr[left] > arr[i] )
            largest = left;
        if( right < arr.size && arr[right] > arr[largest] )
            largest = right;
        //Если больший сын есть, то проталкиваем корень в него.
        if( largest != i ) {
            std::swap( arr[i], arr[largest] );
            siftDown( largest );
        }

    }
};


int main() {
    SerenArray a;
    for (int i = 0; i < 10; ++i) {
        a.pushBack(i);
    }
    for (int i = 0; i < 10; ++i) {
        cout << a[i] << " ";
    }
    return 0;
}

SerenArray::SerenArray() {
    sizeArr = 0;
    size = 0;
    arr = nullptr;
}

SerenArray::SerenArray(int * newArray, size_t n) {
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

SerenArray::~SerenArray() {
    delete[] arr;
    size = 0;
    sizeArr = 0;
}

void SerenArray::pushBack(int element) {
    if (size == sizeArr) {
        int * tempArr = new int[sizeArr * 2];
        memcpy(tempArr, arr, sizeArr * sizeof(int));
        delete[] arr;
        arr = tempArr;
        sizeArr *= 2;
    }
    arr[size++] = element;
}

int & SerenArray::operator[](const int index) {
    return arr[index];
}

bool SerenArray::isEmpty() {
    return (size == 0);
}
