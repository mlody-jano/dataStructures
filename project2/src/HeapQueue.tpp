#ifndef HEAPQUEUE_TPP
#define HEAPQUEUE_TPP
#include "HeapQueue.h"
#endif

template <typename T>
void HeapQueue<T>::enqueue(const Pair<T>& item)
{
    heap.insert(item);
}

template <typename T>
void HeapQueue<T>::extractMax()
{
    heap.extractMax();
}

template <typename T>
const Pair<T>& HeapQueue<T>::peek() const
{
    return heap.findMax();
}

template <typename T>
void HeapQueue<T>::decreaseKey(Pair<T> item, int newPrio)
{
    int index = findIndex(item);

    if (index == -1) {
        throw std::invalid_argument("Item not found in the queue");
    }

    if (newPrio >= heap.at(index).getPriority()) {
        throw std::invalid_argument("New priority must be less than current priority");
    }

    heap.at(index).setPriority(newPrio);
    heap.repairDown(index);
}

template <typename T>
void HeapQueue<T>::increaseKey(Pair<T> item, int newPrio)
{
    int index = findIndex(item);

    if (index == -1) {
        throw std::invalid_argument("Item not found in the queue");
    }

    if (newPrio <= heap.at(index).getPriority()) {
        throw std::invalid_argument("New priority must be greater than current priority");
    }

    heap.at(index).setPriority(newPrio);
    heap.repairUp(index);
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
int HeapQueue<T>::findIndex(const Pair<T>& item) const
{
    for (int i = 0; i < heap.returnSize(); i++) {
        if (heap.at(i).getValue() == item.getValue() && heap.at(i).getPriority() == item.getPriority()) {
            return i;
        }
    }
    return -1;
}