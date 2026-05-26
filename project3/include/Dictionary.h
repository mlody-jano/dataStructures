#pragma once
#include "Pair.h"

template <typename K, typename V>
class Dictionary {
public:

    virtual ~Dictionary() = default;
    Pair<V> insert(const K&, const V&) = 0;
    Pair<V> remove(const K&) = 0;
    Pair<V> find(const K&) const = 0;

    bool exists(const K&) const = 0;
    int size() const = 0;
    bool empty() const = 0;
    
};