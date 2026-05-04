#ifndef PAIR_H
#define PAIR_H

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

template <typename T>
Pair<T>::Pair() : value{}, priority{0} {}

template <typename T>
Pair<T>::Pair(const T& val, int prio) : value{val}, priority{prio} {}

template <typename T>
Pair<T>::~Pair() {}

template <typename T>
void Pair<T>::setValue(const T& temp) {
    value = temp;
}

template <typename T>
void Pair<T>::setPriority(int temp) {
    priority = temp;
}

template <typename T>
T Pair<T>::getValue() const {
    return value;
}

template <typename T>
int Pair<T>::getPriority() const {
    return priority;
}

#endif