#ifndef HEAPQUEUE_TPP
#define HEAPQUEUE_TPP
#include "HeapQueue.h"
#endif

// ── Helpers ──────────────────────────────────────────────────────────────────

template <typename T>
void HeapQueue<T>::swap(int a, int b) {
    Pair<T> tmp = heap[a];
    heap[a] = heap[b];
    heap[b] = tmp;
}

template <typename T>
void HeapQueue<T>::grow() {
    Pair<T>* newHeap = new Pair<T>[capacity * 2];
    for (int i = 0; i < heapSize; i++) newHeap[i] = heap[i];
    delete[] heap;
    heap = newHeap;
    capacity *= 2;
}

template <typename T>
void HeapQueue<T>::heapifyUp(int i) {
    while (i > 0 && heap[parent(i)].getPriority() < heap[i].getPriority()) {
        swap(parent(i), i);
        i = parent(i);
    }
}

template <typename T>
void HeapQueue<T>::heapifyDown(int i) {
    int largest = i;
    int l = left(i), r = right(i);
    if (l < heapSize && heap[l].getPriority() > heap[largest].getPriority()) largest = l;
    if (r < heapSize && heap[r].getPriority() > heap[largest].getPriority()) largest = r;
    if (largest != i) {
        swap(i, largest);
        heapifyDown(largest);
    }
}

template <typename T>
int HeapQueue<T>::findElement(const Pair<T>& item) const {
    for (int i = 0; i < heapSize; i++) {
        if (heap[i].getValue()    == item.getValue() &&
            heap[i].getPriority() == item.getPriority())
            return i;
    }
    return -1;
}

// ── Public interface ──────────────────────────────────────────────────────────

template <typename T>
HeapQueue<T>::HeapQueue() : capacity{100}, heapSize{0}, heap{new Pair<T>[100]} {}

/*
    Copy constructor — deep copy of the internal heap array.
*/
template <typename T>
HeapQueue<T>::HeapQueue(const HeapQueue<T>& other)
    : capacity{other.capacity}, heapSize{other.heapSize},
      heap{new Pair<T>[other.capacity]}
{
    for (int i = 0; i < heapSize; i++)
        heap[i] = other.heap[i];
}

/*
    Copy assignment operator — releases old memory, deep-copies from other.
*/
template <typename T>
HeapQueue<T>& HeapQueue<T>::operator=(const HeapQueue<T>& other)
{
    if (this == &other) return *this;
    delete[] heap;
    capacity  = other.capacity;
    heapSize  = other.heapSize;
    heap      = new Pair<T>[capacity];
    for (int i = 0; i < heapSize; i++)
        heap[i] = other.heap[i];
    return *this;
}

template <typename T>
HeapQueue<T>::~HeapQueue() { delete[] heap; }

template <typename T>
void HeapQueue<T>::enqueue(const Pair<T>& item) {
    if (heapSize == capacity) grow();
    heap[heapSize] = item;
    heapifyUp(heapSize);
    heapSize++;
}

template <typename T>
void HeapQueue<T>::extractMax() {
    if (heapSize == 0) throw std::runtime_error("Queue is empty");
    heap[0] = heap[heapSize - 1];
    heapSize--;
    if (heapSize > 0) heapifyDown(0);
}

template <typename T>
const Pair<T>& HeapQueue<T>::peek() const {
    if (heapSize == 0) throw std::runtime_error("Queue is empty");
    return heap[0];
}

template <typename T>
void HeapQueue<T>::decreaseKey(Pair<T> item, int newPrio) {
    if (newPrio >= item.getPriority())
        throw std::runtime_error("New priority must be lower than current.");
    int pos = findElement(item);
    if (pos == -1) throw std::runtime_error("Element not found in queue.");
    heap[pos].setPriority(newPrio);
    heapifyDown(pos);
}

template <typename T>
void HeapQueue<T>::increaseKey(Pair<T> item, int newPrio) {
    if (newPrio <= item.getPriority())
        throw std::runtime_error("New priority must be higher than current.");
    int pos = findElement(item);
    if (pos == -1) throw std::runtime_error("Element not found in queue.");
    heap[pos].setPriority(newPrio);
    heapifyUp(pos);
}

template <typename T>
bool HeapQueue<T>::isEmpty() const { return heapSize == 0; }

template <typename T>
int HeapQueue<T>::size() const { return heapSize; }