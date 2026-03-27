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

private:
    int* table;
    int capacity;
    int size;
};

#endif