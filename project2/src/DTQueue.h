#ifndef DTQUEUE_H
#define DTQUEUE_H
#include "Queue.h"
#include "DynamicTable.h"
#include "Pair.h"


/*
    Class declaration for a priority queue implemented as a dynamic table.
    Inherits the interface from the Queue class. The implementation is based on a dynamic array, which resizes as needed when elements are added or removed.
    Uses Insertion Sort to maintain the order of elements based on their priority, ensuring that the highest priority element is always at the front of the queue.
*/
template <typename T>
class DTQueue : public Queue<T> {
private:
    DynamicTable<T> data;  // Dynamic array to store the elements of the queue

public:
    DTQueue();
    ~DTQueue();

    void                    enqueue(const Pair<T>& item) override;
    void                    extractMax() override;
    const Pair<T>&          peek() const override;
    void                    decreaseKey(Pair<T> item, int newPrio) override;
    void                    increaseKey(Pair<T> item, int newPrio) override;
    bool                    isEmpty() const override;
    int                     size() const override;
};

template <typename T>
DTQueue<T>::DTQueue() : data{} {}

template <typename T>
DTQueue<T>::~DTQueue() {}

template <typename T>
void DTQueue<T>::enqueue(const Pair<T>& newItem) {
    int currentPos{0};
    while(currentPos < data.returnSize() && 
                            newItem.getPriority() > data.returnElementAtPosition(currentPos).getPriority()) {
        currentPos++;
    }
    data.addElementAtPosition(newItem, currentPos);
}

template <typename T>
void DTQueue<T>::extractMax() {
    if (data.returnSize() == 0) {
        throw runtime_error("Queue is empty");
    }
    data.deleteElementAtEnd();
}

template <typename T>
const Pair<T>& DTQueue<T>::peek() const {
    if (data.returnSize() == 0) {
        throw runtime_error("Queue is empty");
    }
    return data.returnElementAtPosition(data.returnSize() - 1); // Returning the last element because of the fact on table being sorted while inserting elements
};

template <typename T>
void DTQueue<T>::decreaseKey(Pair<T> item, int newPrio) {
    if(item.getPriority() <= newPrio) {
        throw runtime_error("New priority must be lower than current. Use valid operation.");
    }
    int foundPos{-1};
    for(int i{0}; i < data.returnSize(); i++) {
        if (data.returnElementAtPosition(i).getValue() == item.getValue() && data.returnElementAtPosition(i).getPriority() == item.getPriority()) {
            foundPos = i;
            break;
        }
    }
    if(foundPos == -1) {
        throw runtime_error("Element not found in queue.");
    }
    data.deleteElementAtPosition(foundPos);
    item.setPriority(newPrio);
    enqueue(item);
}

template <typename T>
void DTQueue<T>::increaseKey(Pair<T> item, int newPrio) {
    if(item.getPriority() >= newPrio) {
        throw runtime_error("New priority must be higher than current. Use valid operation.");
    }
    int foundPos{-1};
    for(int i{0}; i < data.returnSize(); i++) {
        if (data.returnElementAtPosition(i).getValue() == item.getValue() && data.returnElementAtPosition(i).getPriority() == item.getPriority()) {
            foundPos = i;
            break;
        }
    }
    if(foundPos == -1) {
        throw runtime_error("Element not found in queue.");
    }
    data.deleteElementAtPosition(foundPos);
    item.setPriority(newPrio);
    enqueue(item);
}

template <typename T>
bool DTQueue<T>::isEmpty() const {
    return data.returnSize() == 0;
}

template <typename T>
int DTQueue<T>::size() const {
    return data.returnSize();
}

#endif
