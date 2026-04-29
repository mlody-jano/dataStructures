template <typename T>
class Queue {
public:
    struct Pair {
        T   item;
        int priority;
    };

    virtual void            Queue()                         = default;
    virtual                 ~Queue()                        = default;
    virtual void            enqueue(const Pair& item)       = 0;
    virtual Pair            dequeue()                       = 0;
    virtual const Pair&     front() const                   = 0;
    virtual bool            isEmpty() const                 = 0;
    virtual int             size() const                    = 0;
};