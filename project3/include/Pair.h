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
    int key;
public:
    Pair();
    Pair(const T&, int);
    ~Pair();

    void    setValue(const T&);
    void    setKey(int);
    T       getValue() const;
    int     getKey() const;
};

/**
 * zero argument constructor of class Pair
 */
template <typename T>
Pair<T>::Pair() : value{}, key{0} {}

/**
 * constructor of class Pair
 * @param val specifies the value of Pair, @param key specifies the key of Pair
 */
template <typename T>
Pair<T>::Pair(const T& val, int key) : value{val}, key{key} {}

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
void Pair<T>::setValue(const T& temp) { value = temp;}

/**
 * private setter for key
 * @param temp new key to be changed into
 */
template <typename T>
void Pair<T>::setKey(int temp) { key = temp;}

/**
 * private getter for value
 */
template <typename T>
T Pair<T>::getValue() const { return value;}

/**
 * private getter for key
 */
template <typename T>
int Pair<T>::getKey() const { return key;}

#endif