#ifndef DYNAMICTABLE_H
#define DYNAMICTABLE_H

#include <iostream>

#pragma once

using namespace std;

/*
    Class representing a dynamic table data structure.
    The table is implemented as a dynamic array that can grow in size when needed.
    Stores elements of type T directly (no Pair wrapper).
*/
template <typename T>
class DynamicTable
{
public:

    DynamicTable();
    DynamicTable(const DynamicTable<T>&);
    DynamicTable<T>& operator=(const DynamicTable<T>&);
    ~DynamicTable();

    inline int returnSize()     const { return size; }
    inline int returnCapacity() const { return capacity; }

    void addElementAtBeginning(const T&);
    void addElementAtEnd(const T&);
    void addElementAtPosition(const T&, int);

    void deleteElementAtBeginning();
    void deleteElementAtEnd();
    void deleteElementAtPosition(int);
    void deleteAllElements();

    bool searchElement(const T&) const;

    const T& returnElementAtPosition(int position) const;

    void checkCapacity();

    void display() const;

private:
    int  capacity;
    int  size;
    T*   table;
};

/*
    Constructor — initializes with default capacity of 100, size 0.
*/
template <typename T>
DynamicTable<T>::DynamicTable() : capacity{100}, size{0}, table{new T[capacity]} {}

/*
    Copy constructor — deep copy.
*/
template <typename T>
DynamicTable<T>::DynamicTable(const DynamicTable<T>& other)
    : capacity{other.capacity}, size{other.size}, table{new T[other.capacity]}
{
    for (int i = 0; i < size; i++)
        table[i] = other.table[i];
}

/*
    Copy assignment operator — handles self-assignment safely.
*/
template <typename T>
DynamicTable<T>& DynamicTable<T>::operator=(const DynamicTable<T>& other)
{
    if (this == &other) return *this;
    delete[] table;
    capacity = other.capacity;
    size     = other.size;
    table    = new T[capacity];
    for (int i = 0; i < size; i++)
        table[i] = other.table[i];
    return *this;
}

/*
    Destructor — deallocates the array.
*/
template <typename T>
DynamicTable<T>::~DynamicTable()
{
    delete[] table;
}

/*
    Adds an element at the beginning. O(n) due to shifting.
*/
template <typename T>
void DynamicTable<T>::addElementAtBeginning(const T& element)
{
    checkCapacity();
    for (int i = size; i > 0; i--)
        table[i] = table[i - 1];
    table[0] = element;
    size++;
}

/*
    Adds an element at the end. O(1) amortized.
*/
template <typename T>
void DynamicTable<T>::addElementAtEnd(const T& element)
{
    checkCapacity();
    table[size] = element;
    size++;
}

/*
    Adds an element at a specific position. O(n) due to shifting.
*/
template <typename T>
void DynamicTable<T>::addElementAtPosition(const T& element, int position)
{
    if (position < 0 || position > size)
    {
        cerr << "Invalid position. Element not added." << endl;
        return;
    }
    checkCapacity();
    for (int i = size; i > position; i--)
        table[i] = table[i - 1];
    table[position] = element;
    size++;
}

/*
    Deletes the element at the beginning. O(n) due to shifting.
*/
template <typename T>
void DynamicTable<T>::deleteElementAtBeginning()
{
    if (size == 0)
    {
        cerr << "Table is empty. No element to delete." << endl;
        return;
    }
    for (int i = 0; i < size - 1; i++)
        table[i] = table[i + 1];
    size--;
}

/*
    Deletes the element at the end. O(1).
*/
template <typename T>
void DynamicTable<T>::deleteElementAtEnd()
{
    if (size == 0)
    {
        cerr << "Table is empty. No element to delete." << endl;
        return;
    }
    table[size - 1] = T{};
    size--;
}

/*
    Deletes the element at a specific position. O(n) due to shifting.
*/
template <typename T>
void DynamicTable<T>::deleteElementAtPosition(int position)
{
    if (position < 0 || position >= size)
    {
        cerr << "Invalid position. No element deleted." << endl;
        return;
    }
    for (int i = position; i < size - 1; i++)
        table[i] = table[i + 1];
    size--;
}

/*
    Deletes all elements. O(1) — reinitializes the array.
*/
template <typename T>
void DynamicTable<T>::deleteAllElements()
{
    delete[] table;
    capacity = 100;
    size     = 0;
    table    = new T[capacity];
}

/*
    Searches for an element and prints its position(s) if found. O(n).
*/
template <typename T>
bool DynamicTable<T>::searchElement(const T& element) const
{
    bool found = false;
    for (int i = 0; i < size; i++)
    {
        if (table[i] == element)
        {
            std::cout << "Element found at position " << i << "." << std::endl;
            found = true;
        }
    }
    if (!found)
        std::cout << "Element not found in the table." << std::endl;
    return found;
}

/*
    Doubles capacity when the table is full. O(n).
*/
template <typename T>
void DynamicTable<T>::checkCapacity()
{
    if (size == capacity)
    {
        T* newTable = new T[capacity * 2];
        for (int i = 0; i < size; i++)
            newTable[i] = table[i];
        delete[] table;
        table     = newTable;
        capacity *= 2;
    }
}

/*
    Displays all elements. O(n).
*/
template <typename T>
void DynamicTable<T>::display() const
{
    cout << "Table contents: " << endl;
    for (int i = 0; i < size; i++)
        cout << "[" << i << "] " << table[i] << endl;
}

/*
    Returns the element at a specific position. O(1).
*/
template <typename T>
const T& DynamicTable<T>::returnElementAtPosition(int position) const
{
    if (position < 0 || position >= size)
        throw std::out_of_range("Invalid position.");
    return table[position];
}

#endif