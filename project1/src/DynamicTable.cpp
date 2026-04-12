#include "DynamicTable.h"

/*
    Constructor for DynamicTable
    Initializes the dynamic table with a default capacity of 100 and size of 0
*/
DynamicTable::DynamicTable() : capacity{100}, size{0}, table{new int[capacity]} {}

/*
    Destructor for DynamicTable
    Deallocates the memory used by the dynamic table
*/
DynamicTable::~DynamicTable() {
    delete[] table;
}

/*
    Adds an element at the beginning of the table
    Is O(n) due to the fact we need to shift all the elements to the right to make room for the new element at the beginning of the table
*/
void DynamicTable::addElementAtBeginning(int element) {
    checkCapacity();
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
}

/*
    Adds an element at the end of the table
    Is O(1) due to having the size variable which allows us to add the element at the end of the table without needing to shift any elements
*/
void DynamicTable::addElementAtEnd(int element) {
    checkCapacity();
    table[size] = element;
    size++;
}

/*
    Adds an element at a specific position in the table
    Is O(n) due to the fact we need to shift all the elements to the right of the position to make room for the new element at the given position
*/
void DynamicTable::addElementAtPosition(int element, int position) {
    if (position < 0 || position > capacity)
    {
        cerr << "Invalid position. Element not added." << endl;
        return;
    }
    checkCapacity();

    for (int i = capacity - 1; i > position; i--)
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
void DynamicTable::deleteElementAtBeginning() {
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
void DynamicTable::deleteElementAtEnd() {
    if (size == 0)
    {
        cerr << "Table is empty. No element to delete." << endl;
        return;
    }
    table[size - 1] = NULL; // Deleting last element is O(1) due to having the size variable
    size--;
}

/*
    Deletes an element from a specific position in the table
    Is O(n) due to the fact we need to shift all the elements to the left of the position to fill the gap left by the deleted element at the given position
*/
void DynamicTable::deleteElementAtPosition(int position) {
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
void DynamicTable::deleteAllElements() {
    delete[] table; // Deallocating the current table
    DynamicTable(); // Reinitializing the table to its default state
}

/*
    Searching for an element in the table and printing its position if found
    Is O(n) due to the fact we need to traverse the whole table to find the element
*/
volatile bool DynamicTable::searchElement(int element) const {
    bool found = false;
    for (int i = 0; i < size; i++)
    {
        if (table[i] == element)
        {
            std::cout << "Element " << element << " found at position " << i << "." << std::endl;
            found = true;
        }
    }
    if (!found)
    {
        std::cout << "Element " << element << " not found in the table." << std::endl;
    }
    return found;
}

/*
    Checking if the table has enough capacity to add a new element and doubling the capacity of the table if it is full
    Is O(n) due to copying size elements to new table when the table is full
*/
void DynamicTable::checkCapacity() {
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

/*
    Displaying the elements of the table
    Is O(n) due to the fact we need to traverse the whole table to display all the elements
*/
void DynamicTable::display() const {
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

/*
    Returning the element at a specific position in the table
    Is O(1) due to an array being a contiguous block of memory with indexing 
*/
int DynamicTable::returnElementAtPosition(int position) const {
    if (position < 0 || position >= size)
    {
        cerr << "Invalid position. No element returned." << endl;
        return -1; // Return -1 to indicate an error
    }
    return table[position];
}