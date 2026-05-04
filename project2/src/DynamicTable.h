#ifndef DYNAMICTABLE_H
#define DYNAMICTABLE_H

#include <iostream>
#include "Pair.h"

using namespace std;

/*
    Class representing a dynamic table data structure
    The table is implemented as a dynamic array that can grow in size when needed
*/
template <typename T>
class DynamicTable
{
public:

    DynamicTable();
    ~DynamicTable();

    inline int returnSize() const { return size; };
    inline int returnCapacity() const { return capacity; };

    void addElementAtBeginning(const Pair<T>&);
    void addElementAtEnd(const Pair<T>&);
    void addElementAtPosition(const Pair<T>&, int);

    void deleteElementAtBeginning();
    void deleteElementAtEnd();
    void deleteElementAtPosition(int);
    void deleteAllElements();

    bool searchElement(const Pair<T>&) const;

    const Pair<T>& returnElementAtPosition(int position) const;

    void checkCapacity();

    void display() const; // Utility function to print the contents of the table for testing purposes

private:
    int capacity;
    int size;
    Pair<T> *table;
};

/*
    Constructor for DynamicTable
    Initializes the dynamic table with a default capacity of 100 and size of 0
*/
template <typename T>
DynamicTable<T>::DynamicTable() : capacity{100}, size{0}, table{new Pair<T>[capacity]} {}

/*
    Destructor for DynamicTable
    Deallocates the memory used by the dynamic table
*/
template <typename T>
DynamicTable<T>::~DynamicTable()
{
    delete[] table;
}

/*
    Adds an element at the beginning of the table
    Is O(n) due to the fact we need to shift all the elements to the right to make room for the new element at the beginning of the table
*/
template <typename T>
void DynamicTable<T>::addElementAtBeginning(const Pair<T>& element)
{
    checkCapacity();
    if (size == 0) { // If the table is empty, simply add the element at the first position
        table[0] = element;
        size++;
    }
    for (int i = size; i > 0; i--) {
        table[i] = table[i - 1];
    }

    table[0] = element;
    size++;
}

/*
    Adds an element at the end of the table
    Is O(1) due to having the size variable which allows us to add the element at the end of the table without needing to shift any elements
*/
template <typename T>
void DynamicTable<T>::addElementAtEnd(const Pair<T>& element)
{
    checkCapacity();
    table[size] = element;
    size++;
}

/*
    Adds an element at a specific position in the table
    Is O(n) due to the fact we need to shift all the elements to the right of the position to make room for the new element at the given position
*/
template <typename T>
void DynamicTable<T>::addElementAtPosition(const Pair<T>& element, int position)
{
    if (position < 0 || position > size)    // Not sure whether the addition should be available anywhere in the table, or anywhere between the first and last element, if so, change the size variable in condition to capacity
    {
        cerr << "Invalid position. Element not added." << endl;
        return;
    }
    checkCapacity();

    for (int i = size; i > position; i--)
    { // Shifting elements on the right of position to make room for new element
        table[i] = table[i - 1];
    }
    table[position] = element;
    size++;
}

/*
    Deletes an element from the beginning of the table
    Is O(n) due to the fact we need to shift all the elements to the left to fill the gap left by the deleted element at the beginning of the table
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
    { // Shifting elements on the right to fill the gap left by deleted element
        table[i] = table[i + 1];
    }
    size--;
}

/*
    Deletes an element from the end of the table
    Is O(1) due to having the size variable which allows us to delete the element at the end of the table without needing to shift any elements
*/
template <typename T>
void DynamicTable<T>::deleteElementAtEnd()
{
    if (size == 0)
    {
        cerr << "Table is empty. No element to delete." << endl;
        return;
    }
    table[size - 1] = Pair<T>{}; // Deleting last element is O(1) due to having the size variable
    size--;
}

/*
    Deletes an element from a specific position in the table
    Is O(n) due to the fact we need to shift all the elements to the left of the position to fill the gap left by the deleted element at the given position
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
    { // Shifting elements on the right of position to fill the gap left by deleted element
        table[i] = table[i + 1];
    }
    size--;
}
/*
    Deletes all elements from the table
    Is O(1) due to the fact we can simply deallocate the current table and reinitialize it to its default state without needing to shift any elements
*/
template <typename T>
void DynamicTable<T>::deleteAllElements()
{
    delete[] table; // Deallocating the current table
    capacity = 100;
    size = 0;
    table = new Pair<T>[capacity];
}

/*
    Searching for an element in the table and printing its position if found
    Is O(n) due to the fact we need to traverse the whole table to find the element
*/
template <typename T>
bool DynamicTable<T>::searchElement(const Pair<T>& element) const
{
    bool found = false;
    for (int i = 0; i < size; i++)
    {
        if (table[i].getValue() == element.getValue() && table[i].getPriority() == element.getPriority())
        {
            std::cout << "Element " << element.getValue() << " found at position " << i << "." << std::endl;
            found = true;
        }
    }
    if (!found)
    {
        std::cout << "Element " << element.getValue() << " not found in the table." << std::endl;
    }
    return found;
}

/*
    Checking if the table has enough capacity to add a new element and doubling the capacity of the table if it is full
    Is O(n) due to copying size elements to new table when the table is full
*/
template <typename T>
void DynamicTable<T>::checkCapacity()
{
    if (size == capacity)
    {
        Pair<T> *newTable = new Pair<T>[capacity * 2];
        for (int i = 0; i < size; i++)
        {
            newTable[i] = table[i];
        }
        delete[] table;
        table = newTable;
        capacity *= 2;
    }
}

/*
    Displaying the elements of the table
    Is O(n) due to the fact we need to traverse the whole table to display all the elements
*/
template <typename T>
void DynamicTable<T>::display() const
{
    cout << "Table contents: " << endl;
    for (int i = 0; i < size; i++)
    {
        cout << i << " ";
    }
    cout << endl;

    for (int i = 0; i < size; i++)
    {
        cout << table[i].getValue() << " " << table[i].getPriority() << " | ";
    }
    cout << endl;
}

/*
    Returning the element at a specific position in the table
    Is O(1) due to an array being a contiguous block of memory with indexing
*/
template <typename T>
const Pair<T>& DynamicTable<T>::returnElementAtPosition(int position) const
{
    if (position < 0 || position >= size) {
        throw out_of_range("Invalid position.");
    }
    return table[position];
}

#endif