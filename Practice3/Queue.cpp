#include "Queue.h"
#include <string>
#include <ctime>
#include <stdexcept>

// конструктор по умолчанию
Queue::Queue() {
    size = 0;
    arr = 0;
}

// конструктор для копирования
Queue::Queue(Queue& origQueue) {
    size = origQueue.getSize();
    arr = new int[size];
    for (int i = 0; i < size; i++) {
        arr[i] = origQueue.arr[i];
    }
}

// деструктор
Queue::~Queue() {
    clear();
}

// добавление элемента в конец очереди
void Queue::push(int newValue) {
    size++;
    if (arr)
        arr = (int*) realloc(arr, size*sizeof(int));
    else
        arr = new int[size];
    arr[size-1] = newValue;
}

// показать первое значение в очереди
int Queue::front() {
    checkNull();
    return arr[0];
}

// получить размер очереди
int Queue::getSize() {
    return size;
}

// извлечение первого элемента из очереди
int Queue::pop() {
    checkNull();
    int result = arr[0];
    arr = copyArr(arr, 1);
    return result;
}

// копирование массива при удалении элементов
int *Queue::copyArr(int* oldArr, int count) {
    size -= count;
    int* newArr = new int[size];

    for(int i = 0; i < size; i++)
        newArr[i] = oldArr[count + i];
    delete [] oldArr;
    return newArr;
}

// очистить всю очередь
void Queue::clear() {
    delete [] arr;
    arr = 0;
    size = 0;
}

// очистить очередь до n-ой позиции
void Queue::clear(int pos) {
    checkPos(pos);
    arr = copyArr(arr, pos);
}

// добавление элемента в очередь на n-ую позицию
void Queue::set(int newValue, int pos) {
    checkPos(pos);
    Queue copy(*this);
    clear();
    int copySize = copy.getSize();
    for(int i = 0; i < pos; i++) {
        push(copy.pop());
    }
    copy.pop();
    push(newValue);
    for(int i = pos + 1; i < copySize; i++) {
        push(copy.pop());
    }
}

// получение элемента n-ой позиции
int Queue::get(int pos) {
    checkPos(pos);
    Queue copy(*this);
    copy.clear(pos);
    return copy.front();
}

// сортировка
void Queue::qSort(int left, int right) {
    srand(time(0));
    int pivot = get(rand() % (right - left + 1) + left);
    int i = left;
    int j = right;
    while (i < j) {
        while (pivot > get(i))
            i++;
        while (pivot < get(j))
            j--;
        if(i <= j) {
            int t = get(i);
            set(get(j), i);
            set(t, j);
            i++;
            j--;
        }
    }
    if(i < right)
        qSort(i, right);
    if(j > left)
        qSort(left, j);
}

// проверка на количество элементов в очереди
void Queue::checkNull() {
    if(arr == 0)
        throw std::runtime_error("Очередь пустая");
}

// проверка на выход за пределы очереди
void Queue::checkPos(unsigned int pos) {
    if (pos > size)
        throw std::runtime_error("Значение pos (" + std::to_string(pos) + ") больше чем размер очереди (" + std::to_string(size) + ")");
}
