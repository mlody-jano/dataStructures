#pragma once

template <typename T>
class Heap
{
public:
    Heap();
    ~Heap();

    void insert(const T& item);
    T extractMax();
    const T& findMax() const;

    bool isEmpty() const;
    int returnSize() const;

    T& at(int index);
    const T& at(int index) const;

    void repairUp(int index);
    void repairDown(int index);

private:
    T *data;
    int capacity;
    int size;

    int parent(int index);
    int leftChild(int index);
    int rightChild(int index);

    bool higherPriority(const T& a, const T& b) const;

    void resize();
    void heapifyUp(int index);
    void heapifyDown(int index);
};

#ifndef HEAP_TPP
#define HEAP_TPP
#include "Heap.tpp"
#endif