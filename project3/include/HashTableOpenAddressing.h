#pragma once
#include "Dictionary.h"
#include <stdexcept>

template <typename V>//jeden template bo klucz jest typu int zawsze i tak 
class HashTableOpenAddressing : public Dictionary<int, V> {
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

    int hashFunction(int key) const;
    
    double activeLoadFactor() const;
    double usedLoadFactor() const;

    void resize(int newCapacity);
    void insertWithoutResize(const int& key, const V& value);

public:
    explicit HashTableOpenAddressing(int cap = 101);
    ~HashTableOpenAddressing() override;

    Pair<V> insert(const int& key, const V& value) override;
    Pair<V> remove(const int& key) override;
    Pair<V> find(const int& key) const override;

    bool exists(const int& key) const override;
    int size() const override;
    bool empty() const override;
};

#include "../src/HashTableOpenAddressing.tpp"