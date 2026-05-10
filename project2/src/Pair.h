#ifndef PAIR_H
#define PAIR_H

/**
 * class Pair
 * @tparam T specifies the type of value in pair
 */
template <typename T>
class Pair {
private:
    T value;
    int priority;
public:
    Pair();
    Pair(const T&, int);
    ~Pair();

    void    setValue(const T&);
    void    setPriority(int);
    T       getValue() const;
    int     getPriority() const;
};

/**
 * zero argument constructor of class Pair
 */
template <typename T>
Pair<T>::Pair() : value{}, priority{0} {}

/**
 * constructor of class Pair
 * @param val specifies the value of Pair, @param prio specifies the priority of Pair
 */
template <typename T>
Pair<T>::Pair(const T& val, int prio) : value{val}, priority{prio} {}

/**
 * desctuctor of class Pair
 */
template <typename T>
Pair<T>::~Pair() {}

/**
 * private setter for value
 * @param temp new value to be changed into
 */
template <typename T>
void Pair<T>::setValue(const T& temp) {
    value = temp;
}

/**
 * private setter for priority
 * @param temp new priority to be changed into
 */
template <typename T>
void Pair<T>::setPriority(int temp) {
    priority = temp;
}

/**
 * private getter for value
 */
template <typename T>
T Pair<T>::getValue() const {
    return value;
}

/**
 * private getter for priority
 */
template <typename T>
int Pair<T>::getPriority() const {
    return priority;
}

#endif