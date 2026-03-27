#ifndef DYNAMICTABLE_H
#define DYNAMICTABLE_H

#include <iostream>

#pragma once

using namespace std;

class DynamicTable
{
public:
    DynamicTable();
    ~DynamicTable();

    inline int returnSize() const { return size; };
    inline int returnCapacity() const { return capacity; };

    void addElementAtBeginning(int);
    void addElementAtEnd(int);
    void addElementAtPosition(int, int);

    void deleteElementAtBeginning();
    void deleteElementAtEnd();
    void deleteElementAtPosition(int);

    void searchElement(int) const;

    void checkCapacity();

    void printTable() const; // Utility function to print the contents of the table for testing purposes

private:
    int capacity;
    int size;
    int *table;
};

#endif