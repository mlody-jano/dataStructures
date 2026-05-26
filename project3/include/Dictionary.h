#pragma once
#include "Pair.h"
#include "DynamicTable.h"

/**
 * class Dictionary
 * @tparam K type of keys in the dictionary
 * @tparam V type of values in the dictionary
 * abstract class representing a dictionary data structure that maps keys to according values, creating key-value pairs.
 * later overridden by selected implementations, that being 3 diffrent implementations of hash tables
 */
template <typename K, typename V>
class Dictionary {
    public:
        Dictionary();
        virtual ~Dictionary()                              = default;
        
        void                insert(const K&, const V&)     = 0;
        void                remove(const K&)               = 0;
        void                display() const                = 0;
        bool                empty() const                  = 0;
        bool                exists(const K&) const         = 0;
        int                 size() const                   = 0;
        V                   find(const K&) const           = 0;
        DynamicTable<K>     keys() const                   = 0;
        DynamicTable<V>     values() const                 = 0;
};