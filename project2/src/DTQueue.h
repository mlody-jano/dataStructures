#include "Queue.h"
#include "DynamicTable.h"

/*
    Class declaration for a priority queue implemented as a dynamic table.
    Inherits the interface from the Queue class. The implementation is based on a dynamic array, which resizes as needed when elements are added or removed.
    Uses Insertion Sort to maintain the order of elements based on their priority, ensuring that the highest priority element is always at the front of the queue.
*/

class DTQueue : public Queue<int> {
private:
    DynamicTable<Pair> data;  // Dynamic array to store the elements of the queue

    void insertionSort();

public:
    DTQueue() = default;       // Default constructor
    ~DTQueue() = default;      // Default destructor

    void                    enqueue(const Pair& item) override;
    void                    extractMax() override;
    const Pair&             front() const override;
    void                    decreaseKey(Pair item, int newPrio) override;
    void                    increaseKey(Pair item, int newPrio) override;
    bool                    isEmpty() const override;
    int                     size() const override;
};
