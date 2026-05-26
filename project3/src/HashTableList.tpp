#include "../include/HashTableList.h"

template <typename V>
HashTableList<V>::HashTableList(int cap) : capacity(cap), currentSize(0) {
    table = new Node*[capacity]();
}

template <typename V>
HashTableList<V>::~HashTableList() {
    clear();
    delete[] table;
}

template <typename V>
Pair<V> HashTableList<V>::insert(const int& key, const V& value) {
    int index = hashFunction(key);
    Node* current = table[index];
    while (current != nullptr) {
        if (current->data.getKey() == key) {
            Pair<V> oldValue(current->data.getValue(), key);
            current->data.setValue(value);
            return oldValue;
        }
        current = current->next;
    }
    Pair<V> newPair(value, key);
    table[index] = new Node(newPair, table[index]);
    currentSize++;

    return newPair;
}

template <typename V>
Pair<V> HashTableList<V>::remove(const int& key) {
    int index = hashFunction(key);
    Node* current = table[index];
    Node* prev = nullptr;
    while (current != nullptr) {
        if (current->data.getKey() == key) {
            Pair<V> removedValue = current->data;
            if (prev == nullptr) {
                table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            currentSize--;
            return removedValue;
        }
        prev = current;
        current = current->next;
    }
    return Pair<V>();
}

template <typename V>
Pair<V> HashTableList<V>::find(const int& key) const {
    int index = hashFunction(key);
    Node* current = table[index];
    while (current != nullptr) {
        if (current->data.getKey() == key) {
            return current->data;
        }
        current = current->next;
    }
    return Pair<V>();
}

template <typename V>
bool HashTableList<V>::exists(const int& key) const {
    int index = hashFunction(key);
    Node* current = table[index];
    while (current != nullptr) {
        if (current->data.getKey() == key) {
            return true;
        }
        current = current->next;
    }
    return false;
}

template <typename V>
int HashTableList<V>::size() const {
    return currentSize;
}

template <typename V>
bool HashTableList<V>::empty() const {
    return currentSize == 0;
}

template <typename V>
void HashTableList<V>::clear() {
    for (int i = 0; i < capacity; ++i) {
        Node* current = table[i];
        while (current) {
            Node* toDelete = current;
            current = current->next;
            delete toDelete;
        }
        table[i] = nullptr;
    }
    currentSize = 0;
}