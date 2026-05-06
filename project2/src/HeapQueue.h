#pragma once

#include "Heap.h"
#include "Queue.h"

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
    Heap<Pair<T>> heap;

    int findIndex(const Pair<T>& item) const;
};

#ifndef HEAPQUEUE_TPP
#define HEAPQUEUE_TPP
#include "HeapQueue.tpp"
#endif