#pragma once

#include "Heap.h"
#include "Queue.h"

template <typename T>
class HeapQueue : public Queue<T>
{
public:
    using Pair = typename Queue<T>::Pair;

    HeapQueue() = default;
    ~HeapQueue() override = default;

    void           enqueue(const Pair& item) override;       
    void           extractMax() override;                    
    const Pair&    peek() const override;                   
    void           decreaseKey(Pair item, int newPrio) override;        
    void           increaseKey(Pair item, int newPrio) override;        
    bool           isEmpty() const override;                 
    int            size() const override;                    

private:
    Heap<Pair> heap;

    int findIndex(const Pair& item) const;
};

#ifndef HEAPQUEUE_TPP
#define HEAPQUEUE_TPP
#include "HeapQueue.tpp"
#endif