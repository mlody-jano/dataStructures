#include "DynamicTable.h"

DynamicTable::DynamicTable() : table{new int[capacity]}, capacity{100}, size{0} // Initializing the dynamic table with a default capacity of 100
{
}

DynamicTable::~DynamicTable()
{
    delete[] table;
}

void DynamicTable::addElementAtBeginning(int element) // Adding an element at the beginning of the table
{
    if (size == 0)
    { // If the table is empty, simply add the element at the first position
        table[0] = element;
        size++;
    }
    else if (size < capacity)
    {
        for (int i = size; i > 0; i--)
        {
            table[i] = table[i - 1];
        }
        table[0] = element;
        size++;
    }
    checkCapacity();
}

void DynamicTable::addElementAtEnd(int element) // Adding an element at the end of the table, is 0(1) due to doubling the capacity when table is full
{
    table[size] = element;
    size++;
    checkCapacity();
}

void DynamicTable::addElementAtPosition(int element, int position) // Adding an element at a specific position in the table
{
    if (position < 0 || position > capacity)
    {
        cerr << "Invalid position. Element not added." << endl;
        return;
    }

    if (size == capacity)
    {
        checkCapacity();
    }

    for (int i = capacity - 1; i > position; i--)
    { // Shifting elements on the right of position to make room for new element
        table[i] = table[i - 1];
    }
    table[position] = element;
    size++;
}

void DynamicTable::deleteElementAtBeginning() // Deleting an element from the beginning of the table
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

void DynamicTable::deleteElementAtEnd()
{
    if (size == 0)
    {
        cerr << "Table is empty. No element to delete." << endl;
        return;
    }
    table[size - 1] = NULL; // Deleting last element is O(1) due to having the size variable
    size--;
}

void DynamicTable::deleteElementAtPosition(int position) // Deleting an element from a specific position in the table
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

volatile bool DynamicTable::searchElement(int element) const // Searching for an element in the table and printing its position if found
{
    for (int i = 0; i < size; i++)
    { // Linear search through the table is 0(n) in the worst case, if we have to navigate through the entire table
        if (table[i] == element)
        {
            // cout << "Element " << element << " found at position " << i << "." << endl;
            return true;
        }
    }
    cout << "Element " << element << " not found in the table." << endl;
    return false;
}

void DynamicTable::checkCapacity() // Doubling the capacity of the table. Operation is O(n) due to copying size elements to new table
{
    if (size == capacity)
    {
        int *newTable = new int[capacity * 2];
        for (int i = 0; i < size; i++)
        {
            newTable[i] = table[i];
        }
        delete[] table;
        table = newTable;
        capacity *= 2;
    }
}

void DynamicTable::display() const // Utility function to print the contents of the table for testing purposes
{
    cout << "Table contents: " << endl;
    for (int i = 0; i < size; i++)
    {
        cout << i << " ";
    }
    cout << endl;

    for (int i = 0; i < size; i++)
    {
        cout << table[i] << " ";
    }
    cout << endl;
}

int DynamicTable::returnElementAtPosition(int position) const
    {
        if (position < 0 || position >= size)
        {
            cerr << "Invalid position. No element returned." << endl;
            return -1; // Return -1 to indicate an error
        }
        return table[position];
    }