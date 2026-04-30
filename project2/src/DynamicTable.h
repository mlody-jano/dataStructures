#ifndef DYNAMICTABLE_H
#define DYNAMICTABLE_H

#include <iostream>

#pragma once

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

    void addElementAtBeginning(T);
    void addElementAtEnd(T);
    void addElementAtPosition(int, T);

    void deleteElementAtBeginning();
    void deleteElementAtEnd();
    void deleteElementAtPosition(int);
    void deleteAllElements();

    volatile bool searchElement(T) const;

    T returnElementAtPosition(int position) const;

    void checkCapacity();

    void display() const; // Utility function to print the contents of the table for testing purposes

private:
    int capacity;
    int size;
    T *table;
};

#endif