#ifndef HEAPQUEUE_TPP
#define HEAPQUEUE_TPP
#include "HeapQueue.h"
#endif

template <typename T>
void HeapQueue<T>::enqueue(const Pair& item)
{
    heap.insert(item);
}

template <typename T>
void HeapQueue<T>::extractMax()
{
    heap.extractMax();
}

template <typename T>
const typename HeapQueue<T>::Pair& HeapQueue<T>::peek() const
{
    return heap.findMax();
}

template <typename T>
void HeapQueue<T>::decreaseKey(Pair item, int newPrio)
{
    int index = findIndex(item);
    if (index != -1) {
        heap.at(index).priority = newPrio;
        heap.repairDown(index);
    }
}

template <typename T>
void HeapQueue<T>::increaseKey(Pair item, int newPrio)
{
    int index = findIndex(item);
    if (index != -1) {
        heap.at(index).priority = newPrio;
        heap.repairUp(index);
    }
}

template <typename T>
bool HeapQueue<T>::isEmpty() const
{
    return heap.isEmpty();
}

template <typename T>
int HeapQueue<T>::size() const
{
    return heap.returnSize();
}

template <typename T>
int HeapQueue<T>::findIndex(const Pair& item) const
{
    for (int i = 0; i < heap.returnSize(); i++) {
        if (heap.at(i).item == item.item) {
            return i;
        }
    }
    return -1;
}