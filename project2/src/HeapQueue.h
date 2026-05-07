#pragma once
#include <stdexcept>
#include "Queue.h"
#include "Heap.h"
#include "Pair.h"

/*
    Priority queue implemented as a binary max-heap.
    - enqueue:     O(log n)  — insert at end, heapify up
    - extractMax:  O(log n)  — remove root, heapify down
    - peek:        O(1)      — root is always the maximum
    - decreaseKey: O(n + log n) — linear search + heapify down
    - increaseKey: O(n + log n) — linear search + heapify up
*/
template <typename T>
class HeapQueue : public Queue<T> {
private:
    int      capacity;
    int      heapSize;
    Pair<T>* heap;

    void grow();
    void heapifyUp(int i);
    void heapifyDown(int i);
    void swap(int a, int b);

    int parent(int i) const { return (i - 1) / 2; }
    int left(int i)   const { return 2 * i + 1;   }
    int right(int i)  const { return 2 * i + 2;   }

    int findElement(const Pair<T>& item) const;

public:
    HeapQueue();
    HeapQueue(const HeapQueue<T>&);
    HeapQueue<T>& operator=(const HeapQueue<T>&);
    ~HeapQueue();

    void           enqueue(const Pair<T>& item)           override;
    void           extractMax()                            override;
    const Pair<T>& peek()                           const  override;
    void           decreaseKey(Pair<T> item, int newPrio)  override;
    void           increaseKey(Pair<T> item, int newPrio)  override;
    bool           isEmpty()                        const  override;
    int            size()                           const  override;
};

#ifndef HEAPQUEUE_TPP
#define HEAPQUEUE_TPP
#include "HeapQueue.tpp"
#endif
