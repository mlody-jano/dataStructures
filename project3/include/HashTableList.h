#pragma once

#include "Dictionary.h"

template <typename V>//jeden templaty bo klucz jest typu int zawsze
class HashTableList : public Dictionary<int, V> {
private:
    struct Node {
        Pair<V> data;
        Node* next;
        Node(const Pair<V>& pair) : data(pair), next(nullptr) {}
    };

    Node** table;
    int capacity;
    int currentSize;

    int hashFunction(int key) const {
        return key % capacity;
    }

    void clear();

public:
    explicit HashTableList(int cap = 101);
    ~HashTableList() override;
    
    Pair<V> insert(const int& key, const V& value) override;
    Pair<V> remove(const int& key) override;
    Pair<V> find(const int& key) const override;

    bool exists(const int& key) const override;
    int size() const override;
    bool empty() const override;
};

#include "../src/HashTableList.tpp"
