#pragma once
#include "Dictionary.h"
#include "AVL.h"

template <typename V>
class HashTableAVL : public Dictionary<V> {

    private:
        AVL<V>  **table;
        int     capacity;
        int     count;
        int                 hashFunction(const int&)             const;
    public:
        HashTableAVL(int cap = 101);
        ~HashTableAVL()                                                override;

        void                insert(const int& key, const V& value)     override;
        void                remove(const int& key)                     override;
        void                display()                            const override;
        bool                empty()                              const override {return count == 0 ? true : false;}
        bool                exists(const int&)                   const override;
        int                 size()                               const override {return count;}
        V                   find(const int&)                     const override;
        DynamicTable<int>   keys()                               const;
        DynamicTable<V>     values()                             const;
};

/**
 * non-parameter constructor of @class HashTableAVL
 * @tparam V type of value in node
 */
template <typename V>
HashTableAVL<V>::HashTableAVL(int cap) : capacity(cap), count(0) {
    if (capacity <= 0) { capacity = 101; }

    table = new AVL<V>*[capacity];

    for (int i = 0; i < capacity; ++i) { table[i] = nullptr; }
}

/**
 * destructor of @class HashTableAVL
 * @tparam V type of value in node
 */
template <typename V>
HashTableAVL<V>::~HashTableAVL() {
    for (int i = 0; i < capacity; ++i) { delete table[i]; table[i] = nullptr; }
    count = 0;
    delete[] table;
}

/**
 * hashing function for has table
 * @tparam V type of value in node
 * @tparam num represents the number to be hashed
 */
template <typename V>
int HashTableAVL<V>::hashFunction(const int& num) const {
    int index = num % capacity;
    if (index < 0) { index += capacity; }
    return index;
}

/**
 * insert method for @class HashTableAVL
 * @tparam V type of value in node
 * @param key new key
 * @param value new value
 */
template <typename V>
void HashTableAVL<V>::insert(const int& key, const V& value) {
    unsigned int index = hashFunction(key);

    if (table[index] == nullptr) { table[index] = new AVL<V>(); }

    table[index]->insert(table[index]->getRoot(), key, value);
    count++;
}

/**
 * remove method for @class HashTableAVL
 * @tparam V type of value in node
 * @param key key tied to pair we need to remove
 */
template <typename V>
void HashTableAVL<V>::remove(const int& key) {
    unsigned int index = hashFunction(key);

    if (table[index] == nullptr) { return; }
    table[index]->remove(table[index]->getRoot(), key);
    count--;
}

/**
 * method for displaying the dictionary
 * @tparam V type of value in node
 * uses in-order traversal of tree to showcase the contents
 */

template <typename V>
void HashTableAVL<V>::display() const {
    for (int i = 0; i < capacity; ++i) {
        if (table[i] != nullptr) {
            std::cout << "[" << i << "]: ";
            table[i]->display();
        }
    }
}

/**
 * method for returning a list of keys available in the dictionary
 * @tparam V type of value in node
 * uses in-order traversal to return a list of all keys included in all AVL trees from AVL trees list
 */
template <typename V>
DynamicTable<int> HashTableAVL<V>::keys() const {
    DynamicTable<int> result;
    for (int i = 0; i < capacity; ++i) {
        if (table[i] != nullptr) { table[i]->inorderKeys(table[i]->getRoot(), result); }
    }
    return result;
}

/**
 * method for returning a list of values available in dictionary
 * @tparam V type of value in node
 * uses in-order traversal to return a list of all values included in all AVL trees from AVL trees list
 */
template <typename V>
DynamicTable<V> HashTableAVL<V>::values() const {
    DynamicTable<V> result;
    for (int i = 0; i < capacity; ++i) {
        if (table[i] != nullptr) { table[i]->inorderValues(table[i]->getRoot(), result); }
    }
    return result;
}

/**
 * method for checking if a given key has a value tied to himself
 * @tparam V type of value in node
 * @param key key to be searched for
 */
template <typename V>
bool HashTableAVL<V>::exists(const int& key) const {
    int index = hashFunction(key);

    if (table[index] == nullptr) { return false; }

    try {
        table[index]->find(table[index]->getRoot(), key);
        return true;
    } catch (...) { return false; }
}

/**
 * method for finding a value tied to a given key
 * @tparam V type of value in node
 * @param key key to be searched for
 */
template <typename V>
V HashTableAVL<V>::find(const int& key) const {
    int index = hashFunction(key);

    if (table[index] == nullptr) { throw std::out_of_range("Key not found"); }

    return table[index]->find(table[index]->getRoot(), key);
}