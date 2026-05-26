#pragma once
#include "Pair.h"

template <typename K, typename V>
class Dictionary {
public:

    virtual ~Dictionary() = default;
    virtual Pair<V> insert(const K&, const V&) = 0;
    virtual Pair<V> remove(const K&) = 0;
    virtual Pair<V> find(const K&) const = 0;

    virtual bool exists(const K&) const = 0;
    virtual int size() const = 0;
    virtual bool empty() const = 0;

};