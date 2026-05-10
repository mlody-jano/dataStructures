#ifndef DTQUEUE_H
#define DTQUEUE_H
#include "Queue.h"
#include "DynamicTable.h"
#include "Pair.h"


/**
 * Class DTQueue
 * @tparam T type of value from Pair
 * inherits the interface from the Queue class. the implementation is based on a dynamic array, which resizes as needed when elements are added or removed.
 * inserts the new element in correct position to maintain the order of elements based on their priority.
 */
template <typename T>
class DTQueue : public Queue<T> {
private:
    DynamicTable<T> data;

public:
    DTQueue();
    ~DTQueue();

    void                    enqueue(const Pair<T>& item) override;              // expected: O(n)
    void                    extractMax() override;                              // expected: O(1)
    const Pair<T>&          peek() const override;                              // expected: O(1)
    void                    decreaseKey(Pair<T> item, int newPrio) override;    // expected: O(1)
    void                    increaseKey(Pair<T> item, int newPrio) override;    // expected: O(1)
    bool                    isEmpty() const override;                           // expected: O(1)
    int                     size() const override;                              // expected: O(1)
};

/**
 * non-parameter constructor of class DTQueue
 */
template <typename T>
DTQueue<T>::DTQueue() : data{} {}

/**
 * destructor of class DTQueue
 */
template <typename T>
DTQueue<T>::~DTQueue() {}

/**
    * method that enqueues a new record into queue.
    * @param newItem new pair key-value to be added
    * expected complexity: O(1)
*/
template <typename T>
void DTQueue<T>::enqueue(const Pair<T>& newItem) {
    int currentPos{0};
    while(currentPos < data.returnSize() && 
                            newItem.getPriority() >= data.returnElementAtPosition(currentPos).getPriority()) {
        currentPos++;
    }
    data.addElementAtPosition(newItem, currentPos);
}

/**
 * method that extracts the element with the highest priority
 * expected complexity: O(1)
 */
template <typename T>
void DTQueue<T>::extractMax() {
    if (data.returnSize() == 0) { throw runtime_error("Queue is empty");}   // exception for empty queue
    data.deleteElementAtEnd();                                              // remove the last element because of insertion in enqueue
}

/**
 * method that return the element with the highest priority, without removing it
 * @return modified data DynamicTable implementing the queue
 * expected complexity: O(1)
 */
template <typename T>
const Pair<T>& DTQueue<T>::peek() const {
    if (data.returnSize() == 0) { throw runtime_error("Queue is empty");}   // exception for empty queue
    return data.returnElementAtPosition(data.returnSize() - 1);             // return the last element because of insertion in enqueue
};

/**
 * method that decreases value of key in a specific record in queue
 * searches for item, deletes it, changes priority, and enqueues back
 * @param item represents the record to be found, @param newPrio represents the new Priority to be set (must be lower than current)
 * expected complexity: O(n)
 */
template <typename T>
void DTQueue<T>::decreaseKey(Pair<T> item, int newPrio) {
    if(item.getPriority() <= newPrio) { throw runtime_error("New priority must be lower than current. Use valid operation.");} // expection when new priority is not lower than current
    int foundPos{-1};
    for(int i{0}; i < data.returnSize(); i++) {
        if (data.returnElementAtPosition(i).getValue() == item.getValue() && data.returnElementAtPosition(i).getPriority() == item.getPriority()) {
            foundPos = i;
            break;
        }
    }
    if(foundPos == -1) { throw runtime_error("Element not found in queue.");} // exception when element is not found
    data.deleteElementAtPosition(foundPos);
    item.setPriority(newPrio);
    enqueue(item);
}

/**
 * method that increases value of key in a specific record in queue
 * searches for item, deletes it, changes priority and enqueues back
 * @param item represents the record to be found, @param newPrio represents the new Priority to be set (must be higher than current)
 * expected complexity: O(n)
 */
template <typename T>
void DTQueue<T>::increaseKey(Pair<T> item, int newPrio) {
    if(item.getPriority() >= newPrio) { throw runtime_error("New priority must be higher than current. Use valid operation.");} // exception when new priority is not higher than current
    int foundPos{-1};
    for(int i{0}; i < data.returnSize(); i++) {
        if (data.returnElementAtPosition(i).getValue() == item.getValue() && data.returnElementAtPosition(i).getPriority() == item.getPriority()) {
            foundPos = i;
            break;
        }
    }
    if(foundPos == -1) { throw runtime_error("Element not found in queue.");} // exception when element is not found
    data.deleteElementAtPosition(foundPos);
    item.setPriority(newPrio);
    enqueue(item);
}

/**
 * method that checks whether the queue is empty
 * expected complexity: O(1)
 */
template <typename T>
bool DTQueue<T>::isEmpty() const { return data.returnSize() == 0;}

/**
 * method that checks the size of queue
 * expected complexity: O(1)
 */
template <typename T>
int DTQueue<T>::size() const { return data.returnSize();}

#endif