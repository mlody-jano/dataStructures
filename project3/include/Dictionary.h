#pragma once
#include "Pair.h"
#include "DynamicTable.h"

/**
 * class Dictionary
 * @tparam V type of values in the dictionary
 * abstract class representing a dictionary data structure that maps keys to according values, creating key-value pairs.
 * later overridden by selected implementations, that being 3 diffrent implementations of hash tables
 */
template <typename V>
class Dictionary {
    public:
        Dictionary()                                       = default;
        virtual ~Dictionary()                              = default;

        virtual void                insert(const int& key, const V& value)  = 0;
        virtual void                remove(const int& key)                  = 0;
        virtual void                display() const                         = 0;
        virtual bool                empty() const                           = 0;
        virtual bool                exists(const int& key) const            = 0;
        virtual int                 size() const                            = 0;
        virtual V                   find(const int& key) const              = 0;
};