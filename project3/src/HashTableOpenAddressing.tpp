#include "../include/HashTableOpenAddressing.h"

template <typename V>
HashTableOpenAddressing<V>::HashTableOpenAddressing(int cap) : capacity(cap), currentSize(0), deletedCount(0){
    table = new Slot[capacity];
}

template <typename V>
HashTableOpenAddressing<V>::~HashTableOpenAddressing() {
    delete[] table;
}

template <typename V>
int HashTableOpenAddressing<V>::hashFunction(int key) const {
    return key % capacity;
}

template <typename V>
Pair<V> HashTableOpenAddressing<V>::insert(const int& key, const V& value) {
    if ((currentSize + deletedCount + 1.0) / capacity > 0.7) {//+1 bo uwzglednia element ktory bedzie dodany 
        resize(capacity * 2 + 1);
    }

    int startIndex = hashFunction(key);
    int firstDeletedIndex = -1;

    for (int i = 0; i < capacity; i++) {
        int currentIndex = (startIndex + i) % capacity;

        if (table[currentIndex].state == SlotState::OCCUPIED) {
            if (table[currentIndex].data.getKey() == key) {
                Pair<V> oldPair = table[currentIndex].data;
                table[currentIndex].data.setValue(value);
                return oldPair;
            }
        }
        else if (table[currentIndex].state == SlotState::DELETED) {
            if (firstDeletedIndex == -1) {
                firstDeletedIndex = currentIndex;
            }
        }
        else {
            int insertIndex;

            if (firstDeletedIndex != -1) {
                insertIndex = firstDeletedIndex;
                deletedCount--;
            } else {
                insertIndex = currentIndex;
            }

            table[insertIndex].data = Pair<V>(value, key);
            table[insertIndex].state = SlotState::OCCUPIED;
            currentSize++;

            return table[insertIndex].data;
        }
    }

    if (firstDeletedIndex != -1) {
        table[firstDeletedIndex].data = Pair<V>(value, key);
        table[firstDeletedIndex].state = SlotState::OCCUPIED;
        currentSize++;
        deletedCount--;

        return table[firstDeletedIndex].data;
    }

    resize(capacity * 2 + 1);
    return insert(key, value);
}

template <typename V>
void HashTableOpenAddressing<V>::insertWithoutResize(const int& key, const V& value) {
    int index = hashFunction(key);

    for (int i = 0; i < capacity; i++) {
        int currentIndex = (index + i) % capacity;

        if (table[currentIndex].state != SlotState::OCCUPIED) {
            table[currentIndex].data = Pair<V>(value, key);
            table[currentIndex].state = SlotState::OCCUPIED;
            currentSize++;
            return;
        }
    }

    throw std::overflow_error("Hash table is full during rehash");
}

template <typename V>
Pair<V> HashTableOpenAddressing<V>::remove(const int& key) {
    int startIndex = hashFunction(key);

    for (int i = 0; i < capacity; i++) {
        int currentIndex = (startIndex + i) % capacity;

        if (table[currentIndex].state == SlotState::EMPTY) {
            return Pair<V>();
        }

        if (table[currentIndex].state == SlotState::OCCUPIED &&
            table[currentIndex].data.getKey() == key) {

            Pair<V> removedPair = table[currentIndex].data;
            table[currentIndex].state = SlotState::DELETED;
            currentSize--;
            deletedCount++;

            return removedPair;
        }
    }

    return Pair<V>();
}

template <typename V>
Pair<V> HashTableOpenAddressing<V>::find(const int& key) const {
    int startIndex = hashFunction(key);

    for (int i = 0; i < capacity; i++) {
        int currentIndex = (startIndex + i) % capacity;

        if (table[currentIndex].state == SlotState::EMPTY) {
            return Pair<V>();
        }

        if (table[currentIndex].state == SlotState::OCCUPIED &&
            table[currentIndex].data.getKey() == key) {
            return table[currentIndex].data;
        }
    }

    return Pair<V>();
}

template <typename V>
bool HashTableOpenAddressing<V>::exists(const int& key) const {
    int startIndex = hashFunction(key);

    for (int i = 0; i < capacity; i++) {
        int currentIndex = (startIndex + i) % capacity;

        if (table[currentIndex].state == SlotState::EMPTY) {
            return false;
        }

        if (table[currentIndex].state == SlotState::OCCUPIED &&
            table[currentIndex].data.getKey() == key) {
            return true;
        }
    }

    return false;
}

template <typename V>
int HashTableOpenAddressing<V>::size() const {
    return currentSize;
}

template <typename V>
bool HashTableOpenAddressing<V>::empty() const {
    return currentSize == 0;
}

template <typename V>
double HashTableOpenAddressing<V>::activeLoadFactor() const {
    return static_cast<double>(currentSize) / capacity;
}

template <typename V>
double HashTableOpenAddressing<V>::usedLoadFactor() const{
    return static_cast<double>(currentSize + deletedCount) / capacity;
}

template <typename V>
void HashTableOpenAddressing<V>::resize(int newCapacity) {
    Slot* oldTable = table;
    int oldCapacity = capacity;

    table = new Slot[newCapacity];
    capacity = newCapacity;
    currentSize = 0;
    deletedCount = 0;

    for (int i = 0; i < oldCapacity; i++) {
        if (oldTable[i].state == SlotState::OCCUPIED) {
            insertWithoutResize(
                oldTable[i].data.getKey(),
                oldTable[i].data.getValue()
            );
        }
    }

    delete[] oldTable;
}