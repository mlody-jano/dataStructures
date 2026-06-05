#pragma once

#include "Dictionary.h"
#include "Pair.h"
#include <stdexcept>
#include <iostream>

template <typename V>//jeden templaty bo klucz jest typu int zawsze
class HashTableList : public Dictionary<V> {
private:
    struct Node {
        Pair<V> data;
        Node* next;
        Node(const Pair<V>& pair, Node* nextNode = nullptr) : data(pair), next(nextNode) {}
    };

    Node** table;
    int capacity;
    int currentSize;

    int hashFunction(int key) const;

    void clear();

public:
    explicit HashTableList(int cap = 101);
    ~HashTableList() override;

    HashTableList(const HashTableList&) = delete;
    HashTableList& operator=(const HashTableList&) = delete;
    
    void insert(const int& key, const V& value) override;
    void remove(const int& key) override;
    V find(const int& key) const override;

    bool exists(const int& key) const override;
    int size() const override;
    bool empty() const override;

    void display() const override;
};

#include "../src/HashTableList.tpp"
