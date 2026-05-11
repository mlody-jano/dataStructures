#pragma once

#include "Heap.h"
#include "Queue.h"

/*
    Priority queue implemented as a binary max-heap.
    - enqueue:     O(log n)  — insert at end, heapify up
    - extractMax:  O(log n)  — remove root, heapify down
    - peek:        O(1)      — root is always the maximum
    - decreaseKey: O(n + log n) — linear search + heapify down
    - increaseKey: O(n + log n) — linear search + heapify up
*/

template <typename T>
class HeapQueue : public Queue<T>
{
public:
    HeapQueue() = default;
    ~HeapQueue() override = default;

    void           enqueue(const Pair<T>& item) override;       
    void           extractMax() override;                    
    const Pair<T>&    peek() const override;                   
    void           decreaseKey(Pair<T> item, int newPrio) override;        
    void           increaseKey(Pair<T> item, int newPrio) override;        
    bool           isEmpty() const override;                 
    int            size() const override;                    

private:
    /*To maintain the order of insertion for items with the same priority, 
    we can include an additional field in the Node struct to track the order of insertion. 
    This way, when comparing two nodes with the same priority, we can use their insertion order to determine which one has higher priority 
    (the one that was inserted first).*/
    struct Node {
        Pair<T> pair;
        int order;

        int getPriority() const {
            return pair.getPriority();
        }

        int getOrder() const {
            return order;
        }
    };

    Heap<Node> heap;
    int insertionCounter = 0; 

    int findIndex(const Pair<T>& item) const;
};

#ifndef HEAPQUEUE_TPP
#define HEAPQUEUE_TPP
#include "HeapQueue.tpp"
#endif