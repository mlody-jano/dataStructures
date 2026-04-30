/*
    Class template for a priority queue. Defines the interface, which is later overriden by the derived classes, ListQueue and HeapQueue.
    Includes the definition of Pair structure, for storing a key-value pair.
*/
template <typename T>
class Queue {
public:
    struct Pair {
        T   item;
        int priority;
    };

    virtual                 Queue()                         = default;                    // Default constructor
    virtual                 ~Queue()                        = default;                    // Default destructor
    virtual void            enqueue(const Pair& item)       = 0;                          // Enqueue an item with its priority
    virtual void            extractMax()                    = 0;                          // Extract the item with the highest priority
    virtual const Pair&     peek() const                   = 0;                           // Get the item with the highest priority without removing it
    virtual void            decreaseKey(Pair item, int newPrio)        = 0;               // Decrease the priority of an item
    virtual void            increaseKey(Pair item, int newPrio)        = 0;               // Increase the priority of an item
    virtual bool            isEmpty() const                 = 0;                          // Check if the queue is empty
    virtual int             size() const                    = 0;                          // Get the number of items in the queue
};