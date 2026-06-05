#pragma once
#include "Dictionary.h"
#include "AVL.h"

template <typename V>
class HashTableAVL : public Dictionary<V> {

    private:
        AVL<V>  **table;
        int     capacity;
        int     size;
    protected:
        int hashFunction(const int&) const;
    public:
        HashTableAVL(int cap = 101);
        ~HashTableAVL() override = default;

        void insert(const int& key, const V& value) override;
        void remove(const int& key)                 override;
        void display() const override;
        bool empty() const override {;}
        bool exists(const int&) const override {;}
        int size() const override {;}
        V find(const int&) const override;
        DynamicTable<int> keys() const override;
        DynamicTable<V> values() const override;
};

/**
 * non-parameter constructor of @class HashTableAVL
 * @tparam V type of value in node
 */
template <typename V>
HashTableAVL<V>::HashTableAVL(int cap)
    : capacity(cap), size(0) {

    if (capacity <= 0) {
        capacity = 101;
    }

    table = new AVL<V>*[capacity];

    for (int i = 0; i < capacity; ++i) {
        table[i] = nullptr;
    }
}

/**
 * destructor of @class HashTableAVL
 * @tparam V type of value in node
 */
template <typename V>
HashTableAVL<V>::~HashTableAVL() {
    for (int i = 0; i < capacity; ++i) {
        delete table[i];
        table[i] = nullptr;
    }

    size = 0;
    delete[] table;
}

/**
 * hashing function for has table
 * @tparam V type of value in node
 * @tparam num represents the number to be hashed
 */
template <typename V>
int HashTableAVL<V>::hashFunction(const int& num) { // mistake here, will fix soon
    return;
}