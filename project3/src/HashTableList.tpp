#include "../include/HashTableList.h"

template <typename V>
HashTableList<V>::HashTableList(int cap) : capacity(cap), currentSize(0) {

    if (capacity <= 0) {
        capacity = 101; // default capacity
    }
    table = new Node*[capacity]();
}

template <typename V>
HashTableList<V>::~HashTableList() {
    clear();
    delete[] table;
}

template <typename V>
int HashTableList<V>::hashFunction(int key) const {
    int index = key % capacity;
    if (index < 0) {
        index += capacity;
    }
    return index;
}

// Najpierw przejscie przez caly kubelek Wstawianie z aktualizacją wartości, jeśli klucz już istnieje, lub dodawanie nowego węzła na początku listy dla danego indeksu
template <typename V>
void HashTableList<V>::insert(const int& key, const V& value) {
    int index = hashFunction(key);
    Node* current = table[index];
    while (current != nullptr) {
        if (current->data.getKey() == key) {
            current->data.setValue(value);
            return;
        }
        current = current->next;
    }
    Pair<V> newPair(value, key);
    table[index] = new Node(newPair, table[index]);
    currentSize++;
}

// Usuwanie z przejściem przez listę w kubełku, aktualizacją wskaźników i usuwaniem węzła, jeśli klucz zostanie znaleziony
template <typename V>
void HashTableList<V>::remove(const int& key) {
    int index = hashFunction(key);
    Node* current = table[index];
    Node* prev = nullptr;

    while (current != nullptr) {
        if (current->data.getKey() == key) {
            if (prev == nullptr) {
                table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            currentSize--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

// Znajdowanie z przejściem przez listę w kubełku, zwracaniem wartości, jeśli klucz zostanie znaleziony, lub rzucaniem wyjątku, jeśli klucz nie istnieje
template <typename V>
V HashTableList<V>::find(const int& key) const {
    int index = hashFunction(key);
    Node* current = table[index];

    while (current != nullptr) {
        if (current->data.getKey() == key) {
            return current->data.getValue();
        }
        current = current->next;
    }
    throw std::out_of_range("Key not found in HashTableList");
}

// Sprawdzanie istnienia klucza z przejściem przez listę w kubełku, zwracaniem true, jeśli klucz zostanie znaleziony, lub false, jeśli klucz nie istnieje
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

// Czyszczenie tablicy z usuwaniem wszystkich węzłów i ustawianiem wskaźników na nullptr, oraz resetowaniem rozmiaru
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

// Wyświetlanie stanu tablicy, pokazując indeks i parę klucz-wartość dla każdego elementu w kubełku
template <typename V>
void HashTableList<V>::display() const {
    for (int i = 0; i < capacity; ++i) {
        std::cout << "[" << i << "]";

        Node* current = table[i];

        while (current != nullptr) {
            std::cout << " -> (" << current->data.getKey() << ", " << current->data.getValue() << ")";
            current = current->next;
        }
        std::cout << '\n';
    }
}