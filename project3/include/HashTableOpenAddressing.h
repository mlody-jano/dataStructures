#pragma once
#include "Dictionary.h"
#include "Pair.h"
#include <stdexcept>
#include <iostream>

template <typename V>//jeden template bo klucz jest typu int zawsze i tak 
class HashTableOpenAddressing : public Dictionary<V> {
private:
    enum class SlotState { EMPTY, OCCUPIED, DELETED };

    struct Slot {
        Pair<V> data;
        SlotState state;

        Slot() : data{}, state(SlotState::EMPTY) {}
    };

    Slot* table;
    int capacity;
    int currentSize;
    int deletedCount;
    static constexpr double MAX_USED_LOAD_FACTOR = 0.7;

    int hashFunction(int key) const;
    
    double activeLoadFactor() const;
    double usedLoadFactor() const;

    void resize(int newCapacity);
    void insertWithoutResize(const int& key, const V& value);

public:
    explicit HashTableOpenAddressing(int cap = 101);
    ~HashTableOpenAddressing() override;

    HashTableOpenAddressing(const HashTableOpenAddressing&) = delete;
    HashTableOpenAddressing& operator=(const HashTableOpenAddressing&) = delete;

    void insert(const int& key, const V& value) override;
    void remove(const int& key) override;
    V find(const int& key) const override;

    bool exists(const int& key) const override;
    int size() const override;
    bool empty() const override;

    void display() const override;
};

#include "../src/HashTableOpenAddressing.tpp"