#ifndef HEAP_TPP
#define HEAP_TPP
#include "Heap.h"
#endif

template <typename T>
Heap<T>::Heap() : data(nullptr), capacity(0), size(0) {}

template <typename T>
Heap<T>::Heap(const Heap<T>& other) : data{new T[other.capacity]}, capacity{other.capacity}, size{other.size} {
    for (int i = 0; i < size; i++) {data[i] = other.data[i];}
}

template <typename T>
Heap<T>& Heap<T>::operator=(const Heap<T>& other)
{
    if (this == &other) {return *this;}
    delete[] data;
    data     = new T[capacity];
    capacity = other.capacity;
    size     = other.size;
    for (int i = 0; i < size; i++) {data[i] = other.data[i];}
    return *this;
}

template <typename T>
Heap<T>::~Heap()
{
    delete[] data;
}

template <typename T>
void Heap<T>::insert(const T& item)
{
    if (size == capacity) {
        resize();
    }
    data[size] = item;
    heapifyUp(size);
    size++;
}  

template <typename T>
T Heap<T>::extractMax()
{
    if (isEmpty()) {
        throw std::runtime_error("Heap is empty");
    }
    T maxItem = data[0];
    data[0] = data[size - 1];
    size--;
    heapifyDown(0);
    return maxItem;
}

template <typename T>
const T& Heap<T>::findMax() const
{
    if (isEmpty()) {
        throw std::runtime_error("Heap is empty");
    }
    return data[0];
}

template <typename T>
bool Heap<T>::isEmpty() const
{
    return size == 0;
}

template <typename T>
int Heap<T>::returnSize() const
{
    return size;
}

template <typename T>
T& Heap<T>::at(int index)
{
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

template <typename T>
const T& Heap<T>::at(int index) const
{
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

template <typename T>
void Heap<T>::heapifyUp(int index)
{
    while (index > 0) {
        int parentIndex = parent(index);
        if (higherPriority(data[index], data[parentIndex])) {
            std::swap(data[index], data[parentIndex]);
            index = parentIndex;
        } else {
            break;
        }
    }
}

template <typename T>
void Heap<T>::heapifyDown(int index)
{
    int largest = index;
    int left = leftChild(index);
    int right = rightChild(index);

    if (left < size && higherPriority(data[left], data[largest])) {
        largest = left;
    }
    if (right < size && higherPriority(data[right], data[largest])) {
        largest = right;
    }
    if (largest != index) {
        std::swap(data[index], data[largest]);
        heapifyDown(largest);
    }
}

template <typename T>
void Heap<T>::repairUp(int index)
{
    heapifyUp(index);
}

template <typename T>
void Heap<T>::repairDown(int index)
{
    heapifyDown(index);
}

template <typename T>
int Heap<T>::parent(int index) const
{
    return (index - 1) / 2;
}

template <typename T>
int Heap<T>::leftChild(int index) const
{
    return 2 * index + 1;
}

template <typename T>
int Heap<T>::rightChild(int index) const
{
    return 2 * index + 2;
}

template <typename T>
bool Heap<T>::higherPriority(const T& a, const T& b) const
{
    return a.getPriority() > b.getPriority();
}

template <typename T>
void Heap<T>::resize()
{
    int newCapacity = (capacity == 0) ? 1 : capacity * 2;
    T* newData = new T[newCapacity];
    for (int i = 0; i < size; i++) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
}