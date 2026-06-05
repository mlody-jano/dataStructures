#include "../include/HashTableOpenAddressing.h"

template <typename V>
HashTableOpenAddressing<V>::HashTableOpenAddressing(int cap) : capacity(cap), currentSize(0), deletedCount(0){
    if (capacity <= 0) {
        capacity = 101; // default capacity
    }

    table = new Slot[capacity];
}

template <typename V>
HashTableOpenAddressing<V>::~HashTableOpenAddressing() {
    delete[] table;
}

template <typename V>
int HashTableOpenAddressing<V>::hashFunction(int key) const {
    int index = key % capacity;
    
    if (index < 0) {
        index += capacity;
    }
    return index;
}

template <typename V>
void HashTableOpenAddressing<V>::insert(const int& key, const V& value) {
    int startIndex = hashFunction(key);
    int firstDeletedIndex = -1;
    int emptyIndex = -1;

    for (int i = 0; i < capacity; ++i) {
        int currentIndex = (startIndex + i) % capacity;

        if (table[currentIndex].state == SlotState::OCCUPIED) {
            if (table[currentIndex].data.getKey() == key) {
                table[currentIndex].data.setValue(value);
                return;
            }
        }
        else if (table[currentIndex].state == SlotState::DELETED) {
            if (firstDeletedIndex == -1) {
                firstDeletedIndex = currentIndex;
            }
        }
        else {
            emptyIndex = currentIndex;
            break;
        }
    }

    /*
        klucz nie istnieje
        dodac nowy element
    */

    bool willReuseDeleted = (firstDeletedIndex != -1);

    int usedAfterInsert = currentSize + deletedCount;

    if (!willReuseDeleted) {
        usedAfterInsert++;
    }

    double usedLoadAfterInsert =
        static_cast<double>(usedAfterInsert) / capacity;

    if (usedLoadAfterInsert > MAX_USED_LOAD_FACTOR) {
        /*
            problemem są głównie pola DELETED, można przebudować tablicę o tym samym rozmiarze.
            aktywnych elementów będzie za dużo, trzeba tablicę powiększyć
        */
        double activeLoadAfterInsert =
            static_cast<double>(currentSize + 1) / capacity;

        if (activeLoadAfterInsert <= MAX_USED_LOAD_FACTOR) {
            resize(capacity);
        } else {
            resize(capacity * 2 + 1);
        }

        insertWithoutResize(key, value);
        return;
    }

    int insertIndex;

    if (firstDeletedIndex != -1) {
        insertIndex = firstDeletedIndex;
        deletedCount--;
    } else if (emptyIndex != -1) {
        insertIndex = emptyIndex;
    } else {
        /*
            nie ma EMPTY, ani DELETED, jest pełna
        */
        resize(capacity * 2 + 1);
        insertWithoutResize(key, value);
        return;
    }

    table[insertIndex].data = Pair<V>(value, key);
    table[insertIndex].state = SlotState::OCCUPIED;
    currentSize++;
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
void HashTableOpenAddressing<V>::remove(const int& key) {
    int startIndex = hashFunction(key);

    for (int i = 0; i < capacity; i++) {
        int currentIndex = (startIndex + i) % capacity;

        if (table[currentIndex].state == SlotState::EMPTY) {
            return;
        }

        if (table[currentIndex].state == SlotState::OCCUPIED &&
            table[currentIndex].data.getKey() == key) {

            table[currentIndex].state = SlotState::DELETED;
            currentSize--;
            deletedCount++;

            return;
        }
    }
}

template <typename V>
V HashTableOpenAddressing<V>::find(const int& key) const {
    int startIndex = hashFunction(key);

    for (int i = 0; i < capacity; i++) {
        int currentIndex = (startIndex + i) % capacity;

        if (table[currentIndex].state == SlotState::EMPTY) {
            throw std::out_of_range("Key not found in HashTableOpenAddressing");
        }

        if (table[currentIndex].state == SlotState::OCCUPIED &&
            table[currentIndex].data.getKey() == key) {
            return table[currentIndex].data.getValue();
        }
    }

    throw std::out_of_range("Key not found in HashTableOpenAddressing");
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
    if (newCapacity <= currentSize) {
        newCapacity = currentSize * 2 + 1;
    }

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

template <typename V>
void HashTableOpenAddressing<V>::display() const {
    for (int i = 0; i < capacity; ++i) {
        std::cout << "[" << i << "] ";

        if (table[i].state == SlotState::EMPTY) {
            std::cout << "EMPTY";
        } else if (table[i].state == SlotState::DELETED) {
            std::cout << "DELETED";
        } else {
            std::cout << "(" << table[i].data.getKey() << ", " << table[i].data.getValue() << ")";
        }
        std::cout << '\n';
    }
}